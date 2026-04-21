/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file Camera.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/cameras/Camera.hpp"
#include "raytracer/Struct.hpp"
#include "raytracer/Raytracer.hpp"

void raytracer::Camera::parse(unused const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ShapeDescriptor descriptor;

    // Setup the cframe
    raytracer::Raytracer::setCFrame(descriptor, node);

    // Other settings
    if (!node.exists("resolution"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The resolution field isn't defined for the camera");
    const libconfig::Setting& res = node["resolution"];
    this->_resolution = {
        (int)res[0],
        (int)res[1]
    };

    double fieldOfView = 70.0f;
    if (node.lookupValue("fieldOfView", fieldOfView))
        this->_fieldOfView = fieldOfView;

    // Set the descriptor
    this->setShapeDescriptor(descriptor);
}

void raytracer::Camera::init(void)
{
}

void raytracer::Camera::reset(void)
{
}
