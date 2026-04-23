/**************************************************************\
Edition:
##  @date 23/04/2026 by @author Tsukini

File Name:
##  @file Plane.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#include "utils/utils.hpp"
#include "raytracer/objects/Plane.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"
#include <cmath>

void raytracer::Plane::parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ShapeDescriptor descriptor;

    // Get the object material
    if (!node.exists("material"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The material field isn't defined for the object");
    descriptor.material = raytracer.parseMaterial(node["material"]);

    // Setup the cframe
    raytracer::Raytracer::setCFrame(descriptor, node);
    
    // Other settings
    if (node.exists("dimension")) {
        const libconfig::Setting& dim = node["dimension"];
        descriptor.dimension.x = (int)dim[0];
        descriptor.dimension.y = (int)dim[1];
    } else {
        descriptor.infinite = true;
    }

    // Set the descriptor
    this->setShapeDescriptor(descriptor);
}

float raytracer::Plane::computeSDF(const utils::vector::Vector3<double>& point) const
{
    // Infinite
    if (this->getShapeDescriptor().infinite) {
        return std::abs(point.y - this->getCFrame().position.y);
    }

    // Finite
    else {
    }
}

utils::vector::Vector3<double> raytracer::Plane::computeHit(const utils::vector::Vector3<double>& point) const
{
    return {0.0, 0.0, 0.0};
}
