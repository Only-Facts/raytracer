/**************************************************************\
Edition:
##  @date 23/04/2026 by @author Tsukini

File Name:
##  @file Raytracer.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#define _Vector
#include "utils/utils.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/cameras/Camera.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <iostream>

/*
gui:
    load gui
    loop:
        input handling
        render
        display
    close gui

viewer:
    load gui
    load ppm
    display
    loop:
        input handling
    close gui
*/
void raytracer::Raytracer::gui(void)
{
    // Init screen data from save (viewer mode)
    if (this->_settings.viewer)
        this->loadRender();

    // Open display
    utils::vector::Vector2<std::uint16_t> resolution = this->_camera->getResolution();
    sf::VideoMode mode(resolution.x, resolution.y);
    sf::RenderWindow window(mode, "Raytracer", sf::Style::Titlebar | sf::Style::Close);

    // Pre loop (viewer mode)
    if (this->_settings.viewer)
        this->display(window);

    // Update display
    loop(window);

    // Close display
    window.close();
}

void raytracer::Raytracer::loop(sf::RenderWindow& window)
{
    sf::Event event;

    // Loop while the window is open
    while (window.isOpen()) {

        // Listen event
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            /* other input handle in gui mode */
        }

        // Update display content (gui mode)
        if (this->_settings.gui) {
            this->render();
            this->display(window);
        }
    }
}

void raytracer::Raytracer::display(sf::RenderWindow& window)
{
    // Clear the window
    window.clear(sf::Color::Black);
    
    // Update screen pixels using rays color
    this->_camera->updateScreen();

    // Draw each pixel
    utils::vector::Vector2<std::uint16_t> resolution = this->_camera->getResolution();
    const std::vector<utils::vector::Vector3<std::uint8_t>>& pixels = this->_camera->getScreen();
    for (std::uint16_t y = 0; y < resolution.y; ++y) {
        for (std::uint16_t x = 0; x < resolution.x; ++x) {
            const utils::vector::Vector3<std::uint8_t>& color = pixels[y * resolution.x + x];
            sf::Vertex point(
                sf::Vector2f(x, y),
                sf::Color(color.x, color.y, color.z)
            );
            window.draw(&point, 1, sf::Points);
        }
    }

    // Update the display
    window.display();
}

/*
 1 - Reset rays (camera & lights)
 2 - Compute lights rays
    1 - Compute SDF
    2 - Apply SDF (and aproximative gravity curve, only in newton mode)
    3 - Check SDF
    collision ->
        - Reflect
        - Reduce intensity
        - Apply color fusion
        - Add light rays data (hit point, intensity, color) to the object
    too low intensity -> kill
    too far -> kill
 3 - Compute camera rays
    1 - Compute SDF
    2 - Apply SDF (and aproximative gravity curve, only in newton mode)
    3 - Check SDF
    collision ->
        - Reflect (only for mirror material)
        - Apply color (not for mirror material)
        - kill (not for mirror material)
    too far -> kill
*/
void raytracer::Raytracer::render(void)
{
    std::shared_ptr<raytracer::IObject> nearestObject;
    float sdf = 0.0f, actualSDF = 0.0f;
    bool first = true, edited = true;

    // 1 - Reset rays (camera & lights)
    this->_camera->reset();
    for (std::shared_ptr<raytracer::ILight>& light: this->_lights)
        light->reset();
    for (std::shared_ptr<raytracer::IObject>& object: this->_objects)
        object->clearLightRays();

    // 2 - Compute lights rays
    while (edited) {
        edited = false;
        for (std::shared_ptr<raytracer::ILight>& light: this->_lights) {
            for (std::shared_ptr<raytracer::LightRay>& ray: light->getRays()) {
                if (!ray->isAlive()) continue;
                edited = true;

                // Kill those with no direction
                if (ray->getCFrame().orientation <= 1e-8 && ray->getCFrame().orientation >= -1e-8) {
                    ray->kill();
                    continue;
                }

                // 1 - Compute SDF
                sdf = 0.0f;
                first = true;
                for (std::shared_ptr<raytracer::IObject>& object: this->_objects) {
                    actualSDF = object->computeSDF(ray->getCFrame().position);
                    if (first || actualSDF < sdf) {
                        first = false;
                        sdf = actualSDF;
                        nearestObject = object;
                    }
                }
                if (first) continue;

                // 2 - Apply SDF (and aproximative gravity curve, only in newton mode)
                ray->translate(ray->getCFrame().orientation.normalize() * sdf);

                // 3 - Check SDF
                if (sdf <= SDF_COLLINDING_LIMIT) { // Collision
                    std::cout << "Collide light" << std::endl;
                    nearestObject->reflectRay(ray);
                    nearestObject->addLightRay({ray->getCFrame().position, ray->getColor(), ray->getIntensity()});
                    ray->setIntensity(ray->getIntensity() * nearestObject->getShapeDescriptor().material->getLightReflectionCoef());
                    ray->setColor(
                        std::clamp(
                            static_cast<utils::vector::Vector3<float>>(nearestObject->getShapeDescriptor().material->getColor()).normalize() *
                            static_cast<utils::vector::Vector3<float>>(ray->getColor()).normalize() *
                            nearestObject->getShapeDescriptor().material->getLightReflectionCoef(),
                            {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}
                        ) * 255
                    );
                }

                // Kill conditions
                if (std::isnan(sdf) || (ray->getCFrame().position - this->_camera->getCFrame().position).length() >= RENDER_DISTANCE) { // Too far
                    ray->kill();
                } else if (ray->getIntensity() <= LIGHT_INTENSITY_LIMIT) { // Too low intensity
                    ray->kill();
                }
            }
        }
    }

    // 3 - Compute camera rays
    edited = true;
    while (edited) {
        edited = false;
        for (std::shared_ptr<raytracer::Ray>& ray: this->_camera->getRays()) {
            if (!ray->isAlive()) continue;
            edited = true;

            // Kill those with no direction
            if (ray->getCFrame().orientation <= 1e-8 && ray->getCFrame().orientation >= -1e-8) {
                ray->kill();
                continue;
            }

            // 1 - Compute SDF
            sdf = 0.0f;
            first = true;
            for (std::shared_ptr<raytracer::IObject>& object: this->_objects) {
                actualSDF = object->computeSDF(ray->getCFrame().position);
                if (first || actualSDF < sdf) {
                    first = false;
                    sdf = actualSDF;
                    nearestObject = object;
                }
            }
            if (first) continue;

            // 2 - Apply SDF (and aproximative gravity curve, only in newton mode)
            ray->translate(ray->getCFrame().orientation.normalize() * sdf);

            // 3 - Check SDF
            if (sdf <= SDF_COLLINDING_LIMIT) {
                std::cout << "Collide camera" << std::endl;
                if (nearestObject->getShapeDescriptor().material->isMirror()) { // Mirror material
                    nearestObject->reflectRay(ray);
                } else {
                    ray->setColor(nearestObject->getPointColor(ray->getCFrame().position));
                    ray->kill();
                }
            }

            // Kill conditions
            if (std::isnan(sdf) || (ray->getCFrame().position - this->_camera->getCFrame().position).length() >= RENDER_DISTANCE) { // Too far
                ray->kill();
            }
        }
    }
}

void raytracer::Raytracer::loadRender(void)
{
    // Init default camera (to use default camera comportement)
    this->_camera = std::make_shared<raytracer::Camera>();

    // Try to open the ppm file
    std::ifstream file(this->_settings.ppm_path, std::ios::binary);
    if (!file)
        throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::ppmFile, std::string("Failed to open file for reading: ") + this->_settings.ppm_path);

    // Reading header
    std::uint8_t magic;
    std::uint16_t width, height;
    file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));
    if (!file)
        throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::ppmFile, std::string("Error during the file header reading: ") + this->_settings.ppm_path);

    // Check magic byte
    if (magic != PPM_MAGIC)
        throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::ppmFile, std::string("Invalid magic number: ") + this->_settings.ppm_path);

    // Set the resolution
    this->_camera->setResolution({width, height});
    this->_camera->init(); // Reload rays

    // Read pixels
    std::vector<uint8_t> buffer(width * height * 3);
    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
    std::vector<std::shared_ptr<raytracer::Ray>> rays = this->_camera->getRays();
    for (std::size_t i = 0; i < width * height; ++i)
        rays[i]->setColor({buffer[i * 3 + 0], buffer[i * 3 + 1], buffer[i * 3 + 2]});

    // Check if the reading was successfull
    if (!file)
        throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::ppmFile, std::string("Error during the file reading: ") + this->_settings.ppm_path);
}

void raytracer::Raytracer::saveRender(void)
{
    // Update screen pixels using rays color
    this->_camera->updateScreen();

    // Try to open the ppm file
    std::filesystem::path cfg = this->_settings.cfg_path;
    std::filesystem::path dir = this->_settings.rendered_path;
    cfg.replace_extension(".ppm");
    std::filesystem::path editedPath = dir / cfg.filename();
    std::filesystem::create_directories(editedPath.parent_path());
    std::string path = editedPath.string();
    std::ofstream file(path, std::ios::binary);
    if (!file)
        throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::ppmFile, std::string("Failed to open file for writing: ") + path);

    // Setup header vars
    std::uint8_t magic = PPM_MAGIC;
    utils::vector::Vector2<std::uint16_t> resolution = this->_camera->getResolution();

    // Write file header
    file.write(reinterpret_cast<char*>(&magic), sizeof(magic));
    file.write(reinterpret_cast<char*>(&resolution.x), sizeof(resolution.x));
    file.write(reinterpret_cast<char*>(&resolution.y), sizeof(resolution.y));

    // Write pixels
    const std::vector<utils::vector::Vector3<std::uint8_t>>& pixels = this->_camera->getScreen();
    std::vector<std::uint8_t> buffer;
    buffer.reserve(pixels.size() * 3);
    buffer.resize(pixels.size() * 3);
    for (std::size_t i = 0; i < pixels.size(); ++i) {
        buffer[i * 3 + 0] = pixels[i].x;
        buffer[i * 3 + 1] = pixels[i].y;
        buffer[i * 3 + 2] = pixels[i].z;
    }
    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());

    // Check if the writing was successfull
    if (!file)
        throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::ppmFile, std::string("Error during the file writing: ") + path);
}
