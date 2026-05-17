/**************************************************************\
Edition:
##  @date 17/05/2026 by @author Tsukini

File Name:
##  @file Cylinder.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#include "utils/utils.hpp"
#include "raytracer/objects/Cylinder.hpp"
#include "raytracer/Struct.hpp"
#include <cmath>

void raytracer::Cylinder::parse(const libconfig::Setting& node)
{
    raytracer::ObjectDescriptor descriptor;

    // Setup the cframe & newton
    raytracer::ObjectDescriptor::setCFrame(descriptor, node);
    raytracer::ObjectDescriptor::trySetNewton(descriptor, node);   

    // Other settings
    bool infinite = false;
    if (node.lookupValue("infinite", infinite))
        this->_infinite = infinite;

    double radius = 0.0;
    if (node.lookupValue("radius", radius))
        this->_radius = radius;

    double height = 0.0;
    if (node.lookupValue("height", height))
        this->_height = height;

    // Set the descriptor
    this->setObjectDescriptor(descriptor);
}

hot std::pair<float, const raytracer::Face*> raytracer::Cylinder::computeSDF(const raytracer::Coord& point) const
{
    raytracer::Coord p = point - this->getCFrame().position;
    if (this->_infinite) {
        return {std::sqrt(p.x * p.x + p.z * p.z) - (this->_radius / 2), nullptr};
    } else {
        float dx = std::sqrt(p.x * p.x + p.z * p.z) - (this->_radius / 2);
        float dy = std::abs(p.y) - this->_height * 0.5f;
        float ax = std::max(dx, 0.f);
        float ay = std::max(dy, 0.f);
        return {std::min(std::max(dx, dy), 0.f) + std::sqrt(ax * ax + ay * ay), nullptr};
    }
}

hot raytracer::Direction raytracer::Cylinder::computeHit(unused const raytracer::Coord& point, unused const raytracer::Face* face) const
{
    return {0.0, 0.0, 0.0};
}
