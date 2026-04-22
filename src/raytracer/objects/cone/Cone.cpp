/**************************************************************\
Edition:
##  @date 22/04/2026 by @author Tsukini

File Name:
##  @file Cone.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#include "utils/utils.hpp"
#include "raytracer/objects/Cone.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"

void raytracer::Cone::parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
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
        descriptor.dimension.y = (int)dim[0];
    } else {
        descriptor.infinite = true;
    }

    if (!node.exists("radius"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The radius field isn't defined for the object");
    descriptor.radius = (double)node["radius"];

    // Set the descriptor
    this->setShapeDescriptor(descriptor);
}

float raytracer::Cone::computeSDF(const utils::vector::Vector3<double>& point) const
{
}

utils::vector::Vector3<double> raytracer::Cone::computeHit(const utils::vector::Vector3<double>& point) const
{
}
