/**************************************************************\
Edition:
##  @date 17/05/2026 by @author Tsukini

File Name:
##  @file Cone.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#include "utils/utils.hpp"
#include "raytracer/objects/Cone.hpp"
#include "raytracer/Struct.hpp"

void raytracer::Cone::parse(const libconfig::Setting& node)
{
    raytracer::ObjectDescriptor descriptor;

    // Setup the cframe & newton
    raytracer::ObjectDescriptor::setCFrame(descriptor, node);
    raytracer::ObjectDescriptor::trySetNewton(descriptor, node);   

    // Other settings
    bool infinite = false;
    if (node.lookupValue("infinite", infinite))
        this->_infinite = infinite;

    double angle = 0.0;
    if (node.lookupValue("angle", angle))
        this->_angle = angle;

    double radius = 0.0;
    if (node.lookupValue("radius", radius))
        this->_radius = radius;

    double height = 0.0;
    if (node.lookupValue("height", height))
        this->_height = height;

    // Set the descriptor
    this->setObjectDescriptor(descriptor);
}

hot std::pair<float, const raytracer::Face*> raytracer::Cone::computeSDF(const raytracer::Coord& point) const
{
    raytracer::Coord p = point - this->getCFrame().position;
    if (this->_infinite) {
        float q = std::sqrt(p.x * p.x + p.z * p.z);
        return {std::cos(this->_angle) * q - std::sin(this->_angle) * p.y, nullptr};
    } else {
        float q = std::sqrt(p.x * p.x + p.z * p.z);
        raytracer::Coord k1 = {this->_radius, this->_height, 0};
        raytracer::Coord k2 = {this->_radius, -this->_height, 0};
        float ca_x = q - std::min(q, (p.y < 0.0) ? static_cast<float>(this->_radius) : 0.0f);
        float ca_y = std::abs(p.y) - this->_height;
        float dotk = k1.x * k1.x + k1.y * k1.y;
        float t = std::clamp(((k1.x - q) * k1.x + (k1.y - p.y) * k1.y) / dotk, 0.0, 1.0);
        float cb_x = q - k1.x * t;
        float cb_y = p.y - k1.y * t;
        float s = (cb_x < 0.f && ca_y < 0.f) ? -1.f : 1.f;
        float d1 = ca_x * ca_x + ca_y * ca_y;
        float d2 = cb_x * cb_x + cb_y * cb_y;
        return {s * std::sqrt(std::min(d1, d2)), nullptr};
    }
}

hot raytracer::Direction raytracer::Cone::computeHit(const raytracer::Coord& point, unused const raytracer::Face* face) const
{
    return {0.0, 0.0, 0.0};
}
