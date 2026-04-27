/**************************************************************\
Edition:
##  @date 27/04/2026 by @author Tsukini

File Name:
##  @file Global.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/lights/Global.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"
#include <cstdint>
#include <cstdlib>
#include <cmath>

void raytracer::Global::parse(unused const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ObjectDescriptor descriptor;

    // Set the light to global
    this->enableGlobal();

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

void raytracer::Global::init(void)
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

void raytracer::Global::reset(void)
{
    // For each rays set default light value
    for (raytracer::LightRay* ray: this->_rays) {
        ray->reset();
        ray->setColor(this->_color);
        ray->setIntensity(this->_intensity);
    }
}
