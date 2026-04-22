/**************************************************************\
Edition:
##  @date 22/04/2026 by @author Tsukini

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
    raytracer::ShapeDescriptor descriptor;

    // Setup the cframe
    raytracer::Raytracer::setCFrame(descriptor, node);

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
    this->setShapeDescriptor(descriptor);
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
        this->_rays[i] = std::make_shared<raytracer::LightRay>();
}

void raytracer::Directional::reset(void)
{
    utils::vector::Vector2<std::uint16_t> resolution = {std::sqrt(LIGHT_RAY), std::sqrt(LIGHT_RAY)};
    raytracer::CFrame cframe;
    double aspect, scale;
    double px, py, u, v;

    // For each rays set default light value
    for (std::shared_ptr<raytracer::LightRay>& ray: this->_rays) {
        ray->reset();
        ray->setColor(this->_color);
        ray->setIntensity(this->_intensity);
    }

    // Set rays init position & orientation
    aspect = resolution.x / resolution.y;
    scale = std::tan(this->_fieldOfLight * 0.5 * M_PI / 180.0);
    for (std::size_t y = 0; y < resolution.y; ++y) {
        for (std::size_t x = 0; x < resolution.x; ++x) {
            // Orientation
            u = (x + 0.5) / resolution.x;
            v = (y + 0.5) / resolution.y;
            px = (2.0 * u - 1.0) * aspect * scale;
            py = (1.0 - 2.0 * v) * scale;
            /*
            if (px * px + py * py > 1.0) { // For those who come after... the circle border
                this->_rays[y * resolution.x + x]->kill();
                continue;
            }
            */
            cframe.orientation = {px, py, -1.0};
            cframe.orientation = cframe.orientation.normalize();
            // Position
            cframe.orientation = this->getCFrame().orientation;
            cframe.position = this->getCFrame().position;
            cframe.position.x += x;
            cframe.position.y += y;
            this->_rays[y * resolution.x + x]->setCFrame(cframe);
        }
    }
}
