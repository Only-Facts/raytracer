/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

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
#include "raytracer/Raytracer.hpp"

void raytracer::Cone::parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ShapeDescriptor descriptor;

    // Get the object material
    if (!node.exists("material"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The material field isn't defined for the object");
    descriptor.material = raytracer.parseMaterial(node["material"]);

    // Setup the cframe
    raytracer::Raytracer::setCFrame(descriptor, node);

    /*
    // Other settings
    const libconfig::Setting& dim = node["dimension"];
    descriptor.dimension.x = dim[0];
    descriptor.dimension.y = dim[1];

    float radius = 0.0f;
    mat.lookupValue("radius", radius);
    descriptor.radius = radius;
    */

    // Set the descriptor
    this->setShapeDescriptor(descriptor);
}

void raytracer::Cone::reflectRay(std::shared_ptr<raytracer::IRay> ray) const
{
}

float raytracer::Cone::computeSDF(const utils::vector::Vector3<double>& point) const
{
}
