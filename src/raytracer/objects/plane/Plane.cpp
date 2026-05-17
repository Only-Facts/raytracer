/**************************************************************\
Edition:
##  @date 17/05/2026 by @author Tsukini

File Name:
##  @file Plane.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#include "utils/utils.hpp"
#include "raytracer/objects/Plane.hpp"
#include "raytracer/Struct.hpp"
#include <cmath>

void raytracer::Plane::parse(const libconfig::Setting& node)
{
    raytracer::ObjectDescriptor descriptor;

    // Setup the cframe & newton
    raytracer::ObjectDescriptor::setCFrame(descriptor, node);
    raytracer::ObjectDescriptor::trySetNewton(descriptor, node);   

    // Other settings
    bool infinite = false;
    if (node.lookupValue("infinite", infinite))
        this->_infinite = infinite;

    double width = 0.0;
    if (node.lookupValue("width", width))
        this->_width = width;

    double height = 0.0;
    if (node.lookupValue("height", height))
        this->_height = height;    

    // Set the descriptor
    this->setObjectDescriptor(descriptor);
}

hot std::pair<float, const raytracer::Face*> raytracer::Plane::computeSDF(const raytracer::Coord& point) const
{
    raytracer::Coord p = point - this->getCFrame().position;
    if (this->_infinite) {
        return {p.z, nullptr};
    } else {
        float dx = std::abs(p.x) - this->_width * 0.5f;
        float dz = std::abs(p.z) - this->_height * 0.5f;
        float ax = std::max(dx, 0.f);
        float az = std::max(dz, 0.f);
        return {std::min(std::max(dx, dz), 0.f) + std::sqrt(ax * ax + az * az), nullptr};
    }
}

hot raytracer::Direction raytracer::Plane::computeHit(unused const raytracer::Coord& point, unused const raytracer::Face* face) const
{
    return {0.0, 0.0, 0.0};
}
