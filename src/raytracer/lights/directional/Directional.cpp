/**************************************************************\
Edition:
##  @date 24/04/2026 by @author Tsukini

File Name:
##  @file Directional.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/lights/Directional.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"
#include <cmath>

void raytracer::Directional::parse(unused const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ObjectDescriptor descriptor;

    // Setup the cframe
    raytracer::ObjectDescriptor::setCFrame(descriptor, node);

    // Other settings
    double intensity = 1.0f;
    if (node.lookupValue("intensity", intensity))
        this->_intensity = intensity;

    if (node.exists("color")) {
        const libconfig::Setting& color = node["color"];
        this->_color = {
            (int)color[0],
            (int)color[1],
            (int)color[2]
        };
    }

    double fieldOfLight = 70.0f;
    if (node.lookupValue("fieldOfLight", fieldOfLight))
        this->_fieldOfLight = fieldOfLight;

    // Set the descriptor
    this->setObjectDescriptor(descriptor);
}

void raytracer::Directional::init(void)
{
    std::size_t size = LIGHT_RAY;

    // Clear old data
    this->_rays.clear();

    // Resize screen size
    this->_rays.reserve(size);
    this->_rays.resize(size, nullptr);
    for (std::size_t i = 0; i < size; ++i)
        this->_rays[i] = new raytracer::LightRay();
}

void raytracer::Directional::reset(void)
{
    utils::vector::Vector2<int> resolution = {std::sqrt(LIGHT_RAY), std::sqrt(LIGHT_RAY)};
    raytracer::CFrame cframe;

    // For each rays set default light value
    for (raytracer::LightRay* ray: this->_rays) {
        ray->reset();
        ray->setColor(this->_color);
        ray->setIntensity(this->_intensity);
    }

    // Set rays init position & orientation
    std::cout << "light: " << resolution << std::endl;
    for (int y = 0; y < resolution.y; ++y) {
        for (int x = 0; x < resolution.x; ++x) {
            // Orientation
            cframe.orientation = this->getCFrame().orientation;
            // Position
            cframe.position = this->getCFrame().position;
            cframe.position.x += (x - resolution.x / 2);
            cframe.position.y += (y - resolution.y / 2);
            this->_rays[y * resolution.x + x]->setCFrame(cframe);
        }
    }
}
