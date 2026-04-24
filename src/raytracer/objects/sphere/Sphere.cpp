/**************************************************************\
Edition:
##  @date 24/04/2026 by @author Tsukini

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
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"

cold void raytracer::Sphere::parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ObjectDescriptor descriptor;

    // Get the object material
    if (!node.exists("material"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The material field isn't defined for the object");
    descriptor.material = raytracer.parseMaterial(node["material"]);

    // Setup the cframe
    raytracer::Raytracer::setCFrame(descriptor, node);
    
    // Other settings
    if (!node.exists("radius"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The radius field isn't defined for the object");
    this->_radius = node["radius"];

    // Set the descriptor
    this->setObjectDescriptor(descriptor);
}

hot float raytracer::Sphere::computeSDF(const utils::vector::Vector3<double>& point) const
{
    return (point - this->getObjectDescriptor().cframe.position).length() - this->_radius;
}

hot utils::vector::Vector3<double> raytracer::Sphere::computeHit(const utils::vector::Vector3<double>& point) const
{
    return (point - this->getObjectDescriptor().cframe.position).normalize();
}
