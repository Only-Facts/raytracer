/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file Ambient.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/lights/Ambient.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"
#include <cstdint>

void raytracer::Ambient::parse(unused const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ShapeDescriptor descriptor;

    // Setup the cframe
    raytracer::Raytracer::setCFrame(descriptor, node);

    // Other settings
    float intensity = 1.0f;
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
    this->setShapeDescriptor(descriptor);
}

void raytracer::Ambient::init(void)
{
}

void raytracer::Ambient::reset(void)
{
}
