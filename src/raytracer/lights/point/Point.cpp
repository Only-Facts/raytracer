/**************************************************************\
Edition:
##  @date 27/04/2026 by @author Tsukini

File Name:
##  @file Point.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/lights/Point.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"
#include <cstdint>
#include <cstdlib>
#include <cmath>

void raytracer::Point::parse(unused const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
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

    // Set the descriptor
    this->setObjectDescriptor(descriptor);
}

void raytracer::Point::init(void)
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

static raytracer::Direction randomSphereOrientation(void)
{
    double z = 2.0 * rand() / RAND_MAX - 1.0;
    double t = 2.0 * M_PI * rand() / RAND_MAX;
    double r = std::sqrt(1.0 - z * z);
    return {r * std::cos(t), r * std::sin(t), z};
}

void raytracer::Point::reset(void)
{
    raytracer::CFrame cframe;

    // For each rays set default light value
    for (raytracer::LightRay* ray: this->_rays) {
        ray->reset();
        ray->setColor(this->_color);
        ray->setIntensity(this->_intensity);

        // Generate random spherical orientation
        cframe.orientation = randomSphereOrientation().normalize();
        cframe.position = this->getCFrame().position;
        ray->setCFrame(cframe);
    }
}
