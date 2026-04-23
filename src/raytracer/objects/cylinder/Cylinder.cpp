/**************************************************************\
Edition:
##  @date 23/04/2026 by @author Tsukini

File Name:
##  @file Cylinder.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#include "utils/utils.hpp"
#include "raytracer/objects/Cylinder.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"
#include <cmath>

void raytracer::Cylinder::parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ObjectDescriptor descriptor;

    // Get the object material
    if (!node.exists("material"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The material field isn't defined for the object");
    descriptor.material = raytracer.parseMaterial(node["material"]);

    // Setup the cframe
    raytracer::Raytracer::setCFrame(descriptor, node);
    
    // Other settings
    double scale = 1.0;
    if (node.lookupValue("scale", scale))
        descriptor.scale = scale;

    // Set the descriptor
    this->setObjectDescriptor(descriptor);

    // Load the shape vertex
    this->loadObj(raytracer.ObjPath("cylinder.obj"));
}
