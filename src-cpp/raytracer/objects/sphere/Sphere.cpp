/**************************************************************\
Edition:
##  @date 13/05/2026 by @author Tsukini

File Name:
##  @file Sphere.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#define _Exception
#include "utils/utils.hpp"
#include "raytracer/objects/Sphere.hpp"
#include "raytracer/Struct.hpp"

cold void raytracer::Sphere::parse(const libconfig::Setting& node)
{
    raytracer::ObjectDescriptor descriptor;

    // Setup the cframe
    raytracer::ObjectDescriptor::setCFrame(descriptor, node);
    
    // Other settings
    if (!node.exists("radius"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The radius field isn't defined for the object");
    this->_radius = node["radius"];

    // Set the descriptor
    this->setObjectDescriptor(descriptor);
}

hot std::pair<float, const raytracer::Face*> raytracer::Sphere::computeSDF(const raytracer::Coord& point) const
{
    return {(point - this->getCFrame().position).length() - (this->_radius), nullptr};
}

hot raytracer::Direction raytracer::Sphere::computeHit(const raytracer::Coord& point, unused const raytracer::Face* face) const
{
    return (point - this->getCFrame().position).normalize();
}

hot nodiscard bool raytracer::Sphere::willColide(const raytracer::Coord& point, const raytracer::Direction& orientation) const
{
    raytracer::Direction local = point - this->getCFrame().position;
    raytracer::Type b = local.dot(orientation);
    raytracer::Type c = local.dot(local) - this->_radius * this->_radius;
    return b * b - c >= 0.0;
}
