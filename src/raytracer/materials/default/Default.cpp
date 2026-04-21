/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file Default.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/materials/Default.hpp"
#include "raytracer/Raytracer.hpp"

void raytracer::Default::parse(unused const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
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

    double transparency = 0.f;
    if (node.lookupValue("transparency", transparency))
        this->_transparency = transparency;

    double reflection = 0.f;
    if (node.lookupValue("reflection", reflection))
        this->_reflection = reflection;

    bool mirror = false;
    if (node.lookupValue("is_mirror", mirror))
        this->_mirror = mirror;
}
