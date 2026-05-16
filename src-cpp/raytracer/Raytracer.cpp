/**************************************************************\
Edition:
##  @date 12/05/2026 by @author Tsukini

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
    light
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
    else // Pre light computing
        this->light();

    // Open display
    raytracer::Resolution resolution = this->_camera->getResolution();
    sf::VideoMode mode({resolution.x, resolution.y});
    sf::RenderWindow window(mode, "Raytracer", sf::Style::Titlebar | sf::Style::Close);

    // Pre loop (viewer mode)
    if (this->_settings.viewer)
        this->display(window);

    // Update display
    loop(window);

    // Close display
    window.close();

    // End of the advencement display
    this->advEnd();
}

// Run in a separated thread
static void subLoop(const sf::RenderWindow& window, raytracer::Raytracer& raytracer)
{
    if (!raytracer.isGui()) return;
    while (window.isOpen())
        raytracer.render();
}

void raytracer::Raytracer::loop(sf::RenderWindow& window)
{
    std::thread subThread(subLoop, std::ref(window), std::ref(*this));

    // Loop while the window is open
    while (window.isOpen()) {

        // Listen event
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            /* other input handle in gui mode */
        }

        // Update display content (gui mode)
        if (this->_settings.gui)
            this->display(window);
    }

    // Wait for the thread
    if (subThread.joinable())
        subThread.join();
}

hot void raytracer::Raytracer::display(sf::RenderWindow& window)
{
    // Clear the window
    //window.clear(sf::Color::Black);

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
            window.draw(&point, 1, sf::PrimitiveType::Points);
        }
    }

    // Update the display
    window.display();
}

static hot void processLightChunk(raytracer::Raytracer& raytracer,
    std::vector<raytracer::LightRay*>& rays, std::size_t start, std::size_t end,
    const std::vector<raytracer::IObject*>& objects,
    const std::unordered_map<raytracer::Chunk, std::vector<raytracer::IObject*>, raytracer::ChunkHash>& objectsChunks,
    raytracer::ICamera* camera)
{
    std::vector<raytracer::LightRay*> raysClones;
    raytracer::IObject* nearestObject = nullptr;
    raytracer::Type distanceUnit = 0.0;
    const raytracer::IMaterial* material = nullptr;
    const raytracer::Face* faceHit = nullptr;
    raytracer::Direction orientation;
    raytracer::Angle angle = 0.0;
    float sdf = 0.0;

    for (std::size_t i = start; i < end; ++i) {
        raytracer::LightRay* ray = rays[i];
        distanceUnit = ray->getCFrame().orientation.length();
        ray->computeObjects(camera->getRenderDistance(), objects, objectsChunks);
        while (ray->isAlive()) {
            // Kill those with no direction
            if (ray->getCFrame().orientation <= 1e-8 && ray->getCFrame().orientation >= -1e-8) {
                ray->kill();
                continue;
            }

            // 1 - Compute SDF
            sdf = 0.0f;
            faceHit = nullptr;
            nearestObject = nullptr;
            for (raytracer::IObject* object: ray->getObjects()) {
                //if (ray->getImmunity() == object) continue;
                auto [actualSDF, face] = object->computeSDF(ray->getCFrame().position);
                if (!nearestObject || actualSDF < sdf) {
                    sdf = actualSDF;
                    faceHit = face;
                    nearestObject = object;
                }
            }
            if (!nearestObject) {
                ray->kill();
                continue;
            }

            // 2 - Apply SDF (aproximative gravity curve, only in newton mode)
            ray->translate(ray->getCFrame().orientation * sdf);
            ray->addDistance(distanceUnit * sdf);

            // Kill conditions
            if (std::isnan(sdf) || (ray->getCFrame().position - camera->getCFrame().position).lengthSquared() >= camera->getRenderDistance() * camera->getRenderDistance()) { // Too far
                ray->kill();
            } else if (ray->getDistance() >= camera->getRenderDistance() * RAY_DISTANCE_COEF) { // Live distance
                ray->kill();
            } else if (ray->getIntensity() <= LIGHT_INTENSITY_LIMIT) { // Too low intensity
                ray->kill();
            }

            // 3 - Check SDF
            if (ray->getImmunity() != nearestObject && sdf > -SDF_COLLINDING_LIMIT && sdf < SDF_COLLINDING_LIMIT) { // Collision
                material = nearestObject->getObjectDescriptor().material;
                // Transparency & Refraction
                if (material->getTransparency() > 1e-8) {
                    raysClones.push_back(ray->clone());
                    raysClones.back()->setImmunity(nearestObject);
                    raysClones.back()->setIntensity(raysClones.back()->getIntensity() * material->getTransparency() * (1.0f - material->getLightReflectionCoef()));
                    raysClones.back()->setColor(raytracer::mergeColor(material->getColor(), raysClones.back()->getColor(), raysClones.back()->getIntensity()));
                }

                // Normal computing
                orientation = ray->getCFrame().orientation;
                nearestObject->reflectRay(ray, faceHit);
                ray->computeObjects(camera->getRenderDistance(), objects, objectsChunks);
                angle = raytracer::radToDeg(std::atan2(orientation.dot(ray->getCFrame().orientation), orientation.length() * ray->getCFrame().orientation.length()));
                float localIntensityCoef = 1.0f - (angle / 180.0f); // 0° = 1.0f, 180° = 0.0f
                nearestObject->addLightData(ray->getCFrame().position, ray->getColor(), ray->getLuminescence() * localIntensityCoef * (1.0f - material->getLightReflectionCoef()));
                ray->setIntensity(ray->getIntensity() * material->getLightReflectionCoef());
                ray->setColor(raytracer::mergeColor(material->getColor(), ray->getColor(), ray->getIntensity()));

                // To counter collision with the same object on the next iteration
                ray->translate(ray->getCFrame().orientation * SDF_COLLINDING_LIMIT * 2);
                ray->setImmunity(nearestObject);
            } else if (ray->getImmunity() == nearestObject && sdf > -SDF_COLLINDING_LIMIT && sdf < SDF_COLLINDING_LIMIT) { // Collision on the other side
                nearestObject->addLightData(ray->getCFrame().position, ray->getColor(), ray->getLuminescence());

                // To counter collision with the same object on the next iteration
                ray->translate(ray->getCFrame().orientation * SDF_COLLINDING_LIMIT * 2);
            }

            // Kill conditions
            if (std::isnan(sdf) || (ray->getCFrame().position - camera->getCFrame().position).lengthSquared() >= camera->getRenderDistance() * camera->getRenderDistance()) { // Too far
                ray->kill();
            } else if (ray->getDistance() >= camera->getRenderDistance() * RAY_DISTANCE_COEF) { // Live distance
                ray->kill();
            } else if (ray->getIntensity() <= LIGHT_INTENSITY_LIMIT) { // Too low intensity
                ray->kill();
            }
        }
        raytracer.adv(); // Update advencement
    }

    // Rec for the clonned ones
    if (raysClones.size() > 0) {
        raytracer.advAddMax(raysClones.size()); // Update advencement max
        processLightChunk(raytracer, raysClones, 0, raysClones.size(), objects, objectsChunks, camera);
    }
}

static hot void processCameraChunk(raytracer::Raytracer& raytracer,
    std::vector<raytracer::Ray*>& rays, std::size_t start, std::size_t end,
    const std::vector<raytracer::IObject*>& objects,
    const std::unordered_map<raytracer::Chunk, std::vector<raytracer::IObject*>, raytracer::ChunkHash>& objectsChunks,
    raytracer::ICamera* camera, const raytracer::Sky& sky,
    raytracer::FColor globalLightColor, std::size_t globalLightCount)
{
    raytracer::FColor color = DEFAULT_COLOR;
    raytracer::IObject* nearestObject = nullptr;
    raytracer::Type distanceUnit = 0.0;
    const raytracer::IMaterial* material = nullptr;
    const raytracer::Face* faceHit = nullptr;
    float sdf = 0.0;

    for (std::size_t i = start; i < end; ++i) {
        raytracer::Ray* ray = rays[i];
        ray->computeObjects(camera->getRenderDistance(), objects, objectsChunks);
        distanceUnit = ray->getCFrame().orientation.length();
        while (ray->isAlive()) {
            // Kill those with no direction
            if (ray->getCFrame().orientation <= 1e-8 && ray->getCFrame().orientation >= -1e-8) {
                ray->kill();
                continue;
            }

            // 1 - Compute SDF
            sdf = 0.0f;
            faceHit = nullptr;
            nearestObject = nullptr;
            for (raytracer::IObject* object: ray->getObjects()) {
                if (ray->getImmunity() == object) continue;
                auto [actualSDF, face] = object->computeSDF(ray->getCFrame().position);
                if (!nearestObject || actualSDF < sdf) {
                    sdf = actualSDF;
                    faceHit = face;
                    nearestObject = object;
                }
            }
            if (!nearestObject) { // No valid SDF
                ray->setColor(raytracer::mergeColor(ray->getColor(), sky.getColor(), ray->getCoef()));
                ray->kill();
                continue;
            }

            // 2 - Apply SDF (aproximative gravity curve, only in newton mode)
            ray->translate(ray->getCFrame().orientation * sdf);
            ray->addDistance(distanceUnit * sdf);

            // Kill conditions
            if (std::isnan(sdf) || (ray->getCFrame().position - camera->getCFrame().position).lengthSquared() >= camera->getRenderDistance() * camera->getRenderDistance()) { // Too far
                ray->setColor(raytracer::mergeColor(ray->getColor(), sky.getColor(), ray->getCoef()));
                ray->kill();
            } else if (ray->getDistance() >= camera->getRenderDistance() * RAY_DISTANCE_COEF) { // Live distance
                ray->kill();
            }

            // 3 - Check SDF
            if (sdf > -SDF_COLLINDING_LIMIT && sdf < SDF_COLLINDING_LIMIT) {
                material = nearestObject->getObjectDescriptor().material;
                if (material->isMirror()) { // Mirror material
                    nearestObject->reflectRay(ray, faceHit);
                    ray->computeObjects(camera->getRenderDistance(), objects, objectsChunks);

                    // To counter collision with the same object on the next iteration
                    ray->translate(ray->getCFrame().orientation * SDF_COLLINDING_LIMIT * 2);
                    ray->setImmunity(nearestObject);
                } else {
                    // Normal computing
                    float d = (ray->getCFrame().position - ray->getCFrameOrigin().position).length() / camera->getRenderDistance();
                    float localIntensityCoef = std::exp(-EXP_K * d * d * d * d);
                    auto [pointColor, ok] = nearestObject->getPointColor(ray->getCFrame().position);
                    color = pointColor;

                    // Apply the global light modifier
                    if (globalLightCount > 0 && !ok) color = raytracer::mergeColor(color, raytracer::mergeLight(material->getColor(), globalLightColor, globalLightCount));
                    else if (globalLightCount > 0) color = raytracer::moyColor(color, raytracer::mergeLight(material->getColor(), globalLightColor, globalLightCount));

                    // Generate the noise
                    if (material->hasNoise()) {
                        auto [strength, size] = material->getNoiseSettings();
                        raytracer::noise(ray->getCFrame().position - nearestObject->getCFrame().position, color, strength, size);
                    }

                    // Set the color
                    color = raytracer::mergeColor(ray->getColor(), color, ray->getCoef() * localIntensityCoef * (1.0f - material->getTransparency()));
                    ray->setColor(color);

                    // Transparency & Refraction
                    if (material->getTransparency() > 1e-8) {
                        ray->setCoef(ray->getCoef() * material->getTransparency());

                        // To counter collision with the same object on the next iteration
                        ray->translate(ray->getCFrame().orientation * SDF_COLLINDING_LIMIT * 2);
                        ray->setImmunity(nearestObject);
                    } else {
                        ray->kill();
                    }
                }
            }

            // Kill conditions
            if (std::isnan(sdf) || (ray->getCFrame().position - camera->getCFrame().position).lengthSquared() >= camera->getRenderDistance() * camera->getRenderDistance()) { // Too far
                ray->setColor(raytracer::mergeColor(ray->getColor(), sky.getColor(), ray->getCoef()));
                ray->kill();
            } else if (ray->getDistance() >= camera->getRenderDistance() * RAY_DISTANCE_COEF) { // Live distance
                ray->kill();
            }
        }
        raytracer.adv(); // Update advencement
    }
}

/*
 1 - Reset rays (lights)
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
*/
void raytracer::Raytracer::light(void)
{
    std::vector<std::thread> threads;
    std::size_t countThreads = 1, chunkSize = 1;
    std::size_t start = 0, end = 0;

    // 1 - Reset rays (lights)
    for (raytracer::ILight* light: this->_lights)
        light->reset();
    for (raytracer::IObject* object: this->_objects)
        object->clearLightData();

    // 2 - Compute lights rays
    this->advNext(0);
    for (raytracer::ILight* light: this->_lights) this->advAddMax(light->getRays().size());
    this->adv(true, false);
    for (raytracer::ILight* light: this->_lights) {
        if (light->isGlobal()) continue; // Ignore global light
        std::vector<raytracer::LightRay*> lightRays = light->getRays();
        countThreads = std::thread::hardware_concurrency() - 1;
        chunkSize = lightRays.size() / countThreads;
        threads.clear();
        for (std::size_t i = 0; i < countThreads; ++i) {
            start = i * chunkSize;
            end = (i == countThreads - 1) ? lightRays.size() : start + chunkSize;

            // Start the thread
            threads.emplace_back(processLightChunk,
                std::ref(*this),
                std::ref(lightRays), start, end,
                std::cref(this->_objects),
                std::cref(this->_objectsChunks),
                this->_camera
            );
        }

        // Wait for the threads
        for (std::thread& t: threads)
            t.join();
    }
    this->adv(true, false);
    this->advFull();
    this->advEnd();
    this->advNext(0);
}

/*
 1 - Reset rays (lights)
 2 - Compute color from global lights
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
    raytracer::FColor globalLightColor = DEFAULT_COLOR;
    std::size_t globalLightCount = 0;
    std::vector<std::thread> threads;
    std::size_t countThreads = 1, chunkSize = 1;
    std::size_t start = 0, end = 0;

    // 1 - Reset rays (camera)
    this->_camera->reset();

    // 2 - Compute color from global lights
    for (raytracer::ILight* light: this->_lights) {
        if (!light->isGlobal()) continue; // Ignore no global light
        globalLightColor += light->getColor() * light->getIntensity();
        ++globalLightCount;
    }

    // 3 - Compute camera rays
    std::vector<raytracer::Ray*> cameraRays = this->_camera->getRays();
    countThreads = std::thread::hardware_concurrency() - 1;
    chunkSize = cameraRays.size() / countThreads;
    threads.clear();
    this->advReset(cameraRays.size());
    this->adv(true, false);
    for (std::size_t i = 0; i < countThreads; ++i) {
        start = i * chunkSize;
        end = (i == countThreads - 1) ? cameraRays.size() : start + chunkSize;

        // Start the thread
        threads.emplace_back(processCameraChunk,
            std::ref(*this),
            std::ref(cameraRays), start, end,
            std::cref(this->_objects),
            std::cref(this->_objectsChunks),
            this->_camera, std::cref(this->_sky),
            globalLightColor, globalLightCount
        );
    }

    // Wait for the threads
    for (std::thread& t: threads)
        t.join();
    this->adv(true, false);
    this->advFull();

    // End of the advencement display
    if (!this->_settings.gui)
        this->advEnd();
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
    this->_camera->kill(); // Kill all rays

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
