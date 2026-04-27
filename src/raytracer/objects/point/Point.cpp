/**************************************************************\
Edition:
##  @date 27/04/2026 by @author Tsukini

File Name:
##  @file Point.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#define _Exception
#include "utils/utils.hpp"
#include "raytracer/objects/Point.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"

cold void raytracer::Point::parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ObjectDescriptor descriptor;

    // Get the object material
    if (!node.exists("material"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The material field isn't defined for the object");
    descriptor.material = raytracer.parseMaterial(node["material"]);

    // Setup the cframe
    raytracer::ObjectDescriptor::setCFrame(descriptor, node);

    // Get the camera
    this->_camera = raytracer.getCamera();

    // Set the descriptor
    this->setObjectDescriptor(descriptor);
}

hot std::pair<float, const raytracer::Face*> raytracer::Point::computeSDF(const utils::vector::Vector3<double>& point) const
{
    return {(point - this->getObjectDescriptor().cframe.position).length(), nullptr};
}

hot utils::vector::Vector3<double> raytracer::Point::computeHit(const utils::vector::Vector3<double>& point, unused const raytracer::Face* face) const
{
    if (this->_camera)
        return (this->_camera->getCFrame().position - point).normalize(); // Camera direction
    else 
        return {0.0, 0.0, 0.0}; // Nop
}
