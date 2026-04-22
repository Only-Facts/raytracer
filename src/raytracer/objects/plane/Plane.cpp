/**************************************************************\
Edition:
##  @date 22/04/2026 by @author Tsukini

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
    if (!node.exists("dimension"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The dimension field isn't defined for the object");
    const libconfig::Setting& dim = node["dimension"];
    descriptor.dimension.x = (int)dim[0];
    descriptor.dimension.y = (int)dim[1];

    // Set the descriptor
    this->setShapeDescriptor(descriptor);
}

float raytracer::Plane::computeSDF(const utils::vector::Vector3<double>& point) const
{
}

utils::vector::Vector3<double> raytracer::Plane::computeHit(const utils::vector::Vector3<double>& point) const
{
}
