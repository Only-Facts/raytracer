/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

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

    // Set the descriptor
    this->setShapeDescriptor(descriptor);
}

void raytracer::Directional::init(void)
{
}

void raytracer::Directional::reset(void)
{
}
