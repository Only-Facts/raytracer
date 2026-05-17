/**************************************************************\
Edition:
##  @date 17/05/2026 by @author Tsukini

File Name:
##  @file Noise.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/shaders/Noise.hpp"
#include "raytracer/special/Utils.hpp"

void raytracer::Noise::parse(const libconfig::Setting& node)
{
    /* noise */
    double strength = 0.0;
    if (node.lookupValue("strength", strength))
        this->_strength = strength;

    double size = 0.0;
    if (node.lookupValue("size", size))
        this->_size = size;
}

hot nodiscard raytracer::Color raytracer::Noise::scene(unused const raytracer::Coord& worldPoint, const raytracer::Coord& localPoint, const raytracer::Color& color)
{
    raytracer::FColor fcolor = color;
    raytracer::noise(localPoint, fcolor, this->_strength, this->_size);
    return fcolor;
}
