/**************************************************************\
Edition:
##  @date 30/04/2026 by @author Tsukini

File Name:
##  @file Raytracer.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#define _Vector
#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/special/Utils.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/cameras/Viewer.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <cstddef>
#include <cstdint>
#include <thread>
#include <vector>
#include <cmath>

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
cold void raytracer::Raytracer::gui(void)
{
    // Init screen data from save (viewer mode)
    if (this->_settings.viewer)
        this->loadRender();

    // Open display
    raytracer::Resolution resolution = this->_camera->getResolution();
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

hot void raytracer::Raytracer::display(sf::RenderWindow& window)
{
    // Clear the window
    window.clear(sf::Color::Black);
    
    // Update screen pixels using rays color
    this->_camera->updateScreen();

    // Draw each pixel
    raytracer::Resolution resolution = this->_camera->getResolution();
    const std::vector<raytracer::Color>& pixels = this->_camera->getScreen();
    for (std::uint16_t y = 0; y < resolution.y; ++y) {
        for (std::uint16_t x = 0; x < resolution.x; ++x) {
            const raytracer::Color& color = pixels[y * resolution.x + x];
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

static hot void processLightChunk(std::vector<raytracer::LightRay*>& rays,
    size_t start, size_t end,
    const std::vector<raytracer::IObject*>& objects,
    raytracer::ICamera* camera)
{
    raytracer::IObject* nearestObject = nullptr;
    const raytracer::Face* faceHit = nullptr;
    float sdf = 0.0;
    bool first = true;

    for (std::size_t i = start; i < end; ++i) {
        raytracer::LightRay* ray = rays[i];
        while (ray->isAlive()) {
            // Kill those with no direction
            if (ray->getCFrame().orientation <= 1e-8 && ray->getCFrame().orientation >= -1e-8) {
                ray->kill();
                continue;
            }

            // 1 - Compute SDF
            sdf = 0.0f;
            first = true;
            for (raytracer::IObject* object: objects) {
                auto [actualSDF, face] = object->computeSDF(ray->getCFrame().position);
                if (first || actualSDF < sdf) {
                    first = false;
                    sdf = actualSDF;
                    faceHit = face;
                    nearestObject = object;
                  }
            }
            if (first) continue;

            // 2 - Apply SDF (and aproximative gravity curve, only in newton mode)
            ray->translate(ray->getCFrame().orientation * sdf);

            // 3 - Check SDF
            if (sdf <= SDF_COLLINDING_LIMIT) { // Collision
                nearestObject->reflectRay(ray, faceHit);
                nearestObject->addLightData(ray->getCFrame().position, ray->getColor(), ray->getIntensity());
                ray->setIntensity(ray->getIntensity() * (nearestObject->getObjectDescriptor().material->isMirror() ? 1.0f : nearestObject->getObjectDescriptor().material->getLightReflectionCoef()));
                ray->setColor(raytracer::mergeColor(nearestObject->getObjectDescriptor().material->getColor(), ray->getColor(), ray->getIntensity()));

                // To counter collision with the same object on the next iteration
                ray->translate(ray->getCFrame().orientation * (SDF_COLLINDING_LIMIT + 1));
            }

            // Kill conditions
            if (std::isnan(sdf) || (ray->getCFrame().position - camera->getCFrame().position).length() >= camera->getRenderDistance()) { // Too far
                ray->kill();
            } else if (ray->getIntensity() <= LIGHT_INTENSITY_LIMIT) { // Too low intensity
                ray->kill();
            }
        }
    }
}

static hot void processCameraChunk(std::vector<raytracer::Ray*>& rays,
    size_t start, size_t end,
    const std::vector<raytracer::IObject*>& objects,
    raytracer::ICamera* camera, const raytracer::Sky& sky,
    raytracer::Color globalLightColor)
{
    raytracer::Color color = DEFAULT_COLOR;
    raytracer::IObject* nearestObject = nullptr;
    const raytracer::Face* faceHit = nullptr;
    float sdf = 0.0;
    bool first = true;

    for (std::size_t i = start; i < end; ++i) {
        raytracer::Ray* ray = rays[i];
        while (ray->isAlive()) {
            // Kill those with no direction
            if (ray->getCFrame().orientation <= 1e-8 && ray->getCFrame().orientation >= -1e-8) {
                ray->kill();
                continue;
            }

            // 1 - Compute SDF
            sdf = 0.0f;
            first = true;
            for (raytracer::IObject* object: objects) {
                auto [actualSDF, face] = object->computeSDF(ray->getCFrame().position);
                if (first || actualSDF < sdf) {
                    first = false;
                    sdf = actualSDF;
                    faceHit = face;
                    nearestObject = object;
                }
            }
            if (first) continue;

            // 2 - Apply SDF (and aproximative gravity curve, only in newton mode)
            ray->translate(ray->getCFrame().orientation * sdf);

            // 3 - Check SDF
            if (sdf <= SDF_COLLINDING_LIMIT) {
                if (nearestObject->getObjectDescriptor().material->isMirror()) { // Mirror material
                    nearestObject->reflectRay(ray, faceHit);

                    // To counter collision with the same object on the next iteration
                    ray->translate(ray->getCFrame().orientation * (SDF_COLLINDING_LIMIT + 1));
                } else {
                    float localIntensityCoef = std::exp(-EXP_K * (ray->getCFrame().position - ray->getCFrameOrigin().position).length() / camera->getRenderDistance());
                    color = nearestObject->getPointColor(ray->getCFrame().position);
                    color = raytracer::mergeColor((color == 0) ? nearestObject->getObjectDescriptor().material->getColor() : color, globalLightColor, localIntensityCoef);
                    ray->setColor(color);
                    ray->kill();
                }
            }

            // Kill conditions
            if (std::isnan(sdf) || (ray->getCFrame().position - camera->getCFrame().position).length() >= camera->getRenderDistance()) { // Too far
                ray->setColor(sky.getColor());
                ray->kill();
            }
        }
    }
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
 3 - Compute color from global lights
 4 - Compute camera rays
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
    raytracer::Color globalLightColor = {255, 255, 255}; // White to allow any other color
    std::vector<std::thread> threads;
    std::size_t countThreads = 1, chunkSize = 1;
    std::size_t start = 0, end = 0;

    // 1 - Reset rays (camera & lights)
    this->_camera->reset();
    for (raytracer::ILight* light: this->_lights)
        light->reset();
    for (raytracer::IObject* object: this->_objects)
        object->clearLightData();

    // 2 - Compute lights rays
    for (raytracer::ILight* light: this->_lights) {
        if (light->isGlobal()) continue; // Ignore global light
        std::vector<raytracer::LightRay*> lightRays = light->getRays();
        countThreads = std::thread::hardware_concurrency();
        chunkSize = lightRays.size() / countThreads;
        threads.clear();
        for (std::size_t i = 0; i < countThreads; ++i) {
            start = i * chunkSize;
            end = (i == countThreads - 1) ? lightRays.size() : start + chunkSize;

            // Start the thread
            threads.emplace_back(processLightChunk,
                std::ref(lightRays),
                start, end,
                std::cref(this->_objects),
                this->_camera
            );
        }

        // Wait for the threads
        for (std::thread& t: threads)
            t.join();
    }

    // 3 - Compute color from global lights
    for (raytracer::ILight* light: this->_lights) {
        if (!light->isGlobal()) continue; // Ignore no global light
        globalLightColor = raytracer::mergeColor(globalLightColor, light->getColor(), light->getIntensity());
    }

    // 3 - Compute camera rays
    std::vector<raytracer::Ray*> cameraRays = this->_camera->getRays();
    countThreads = std::thread::hardware_concurrency();
    chunkSize = cameraRays.size() / countThreads;
    threads.clear();
    for (std::size_t i = 0; i < countThreads; ++i) {
        start = i * chunkSize;
        end = (i == countThreads - 1) ? cameraRays.size() : start + chunkSize;

        // Start the thread
        threads.emplace_back(processCameraChunk,
            std::ref(cameraRays),
            start, end,
            std::cref(this->_objects),
            this->_camera, std::cref(this->_sky),
            globalLightColor
        );
    }

    // Wait for the threads
    for (std::thread& t: threads)
        t.join();
}

cold void raytracer::Raytracer::loadRender(void)
{
    // Init default camera (to use default camera comportement)
    this->_camera = new raytracer::Viewer();

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
    std::vector<raytracer::Ray*> rays = this->_camera->getRays();
    for (std::size_t i = 0; i < width * height; ++i)
        rays[i]->setColor({buffer[i * 3 + 0], buffer[i * 3 + 1], buffer[i * 3 + 2]});

    // Check if the reading was successfull
    if (!file)
        throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::ppmFile, std::string("Error during the file reading: ") + this->_settings.ppm_path);
}

cold void raytracer::Raytracer::saveRender(void)
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
    raytracer::Resolution resolution = this->_camera->getResolution();

    // Write file header
    file.write(reinterpret_cast<char*>(&magic), sizeof(magic));
    file.write(reinterpret_cast<char*>(&resolution.x), sizeof(resolution.x));
    file.write(reinterpret_cast<char*>(&resolution.y), sizeof(resolution.y));

    // Write pixels
    const std::vector<raytracer::Color>& pixels = this->_camera->getScreen();
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
