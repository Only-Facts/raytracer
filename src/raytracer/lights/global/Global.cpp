/**************************************************************\
Edition:
##  @date 02/05/2026 by @author Tsukini

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
    /* nothing */
}

void raytracer::Global::reset(void)
{
    /* nothing */
}
