/**************************************************************\
Edition:
##  @date 23/04/2026 by @author Tsukini

File Name:
##  @file Point.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#include "utils/utils.hpp"
#include "raytracer/objects/Point.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"

void raytracer::Point::parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ShapeDescriptor descriptor;

    // Get the object material
    if (!node.exists("material"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The material field isn't defined for the object");
    descriptor.material = raytracer.parseMaterial(node["material"]);

    // Setup the cframe
    raytracer::Raytracer::setCFrame(descriptor, node);
    
    // Set the descriptor
    this->setShapeDescriptor(descriptor);
}

float raytracer::Point::computeSDF(const utils::vector::Vector3<double>& point) const
{
    return (point - this->getCFrame().position).length();
}

utils::vector::Vector3<double> raytracer::Point::computeHit(const utils::vector::Vector3<double>& point) const
{
    return (point - this->getCFrame().position).normalize();
}
