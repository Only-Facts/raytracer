/**************************************************************\
Edition:
##  @date 13/05/2026 by @author Tsukini

File Name:
##  @file Default.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/materials/Default.hpp"

void raytracer::Default::parse(const libconfig::Setting& node)
{
    // Other settings
    if (node.exists("color")) {
        const libconfig::Setting& color = node["color"];
        this->_color = {
            (int)color[0],
            (int)color[1],
            (int)color[2]
        };
    }

    /* surface */
    double transparency = 0.f;
    if (node.lookupValue("transparency", transparency))
        this->_transparency = transparency;

    double reflection = 0.f;
    if (node.lookupValue("reflection", reflection))
        this->_reflection = reflection;

    /* noise */
    bool noise = false;
    if (node.lookupValue("noise", noise))
        this->_noise = noise;

    double strength = 0.0;
    if (node.lookupValue("strength", strength))
        this->_strength = strength;

    double size = 0.0;
    if (node.lookupValue("size", size))
        this->_size = size;
}
