/**************************************************************\
Edition:
##  @date 25/04/2026 by @author Tsukini

File Name:
##  @file CameraFOV.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/cameras/CameraFOV.hpp"
#include "raytracer/Struct.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/rays/Ray.hpp"
#include <limits>
#include <cmath>

void raytracer::CameraFOV::parse(unused const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ObjectDescriptor descriptor;

    // Setup the cframe
    raytracer::ObjectDescriptor::setCFrame(descriptor, node);

    // Other settings
    if (!node.exists("resolution"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The resolution field isn't defined for the camera");
    const libconfig::Setting& res = node["resolution"];
    this->setResolution({(int)res[0], (int)res[1]});

    double fieldOfView = 70.0f;
    if (node.lookupValue("fieldOfView", fieldOfView))
        this->_fieldOfView = fieldOfView;

    // Set the descriptor
    this->setObjectDescriptor(descriptor);
}

void raytracer::CameraFOV::init(void)
{
    std::size_t size = static_cast<std::size_t>(this->getResolution().x) * this->getResolution().y;

    // Clear old data
    this->_screen.clear();
    this->_rays.clear();

    // Resize screen size
    this->_screen.reserve(size);
    this->_rays.reserve(size);
    this->_screen.resize(size, utils::vector::Vector3<std::uint8_t>DEFAULT_COLOR);
    this->_rays.resize(size, nullptr);
    for (std::size_t i = 0; i < size; ++i)
        this->_rays[i] = new raytracer::Ray();
}

void raytracer::CameraFOV::reset(void)
{
    utils::vector::Vector2<int> resolution;
    raytracer::CFrame cframe;

    // For each rays set default light value
    for (raytracer::Ray* ray: this->_rays)
        ray->reset();

    // Set rays init position & orientation
    resolution = this->getResolution();
    for (int y = 0; y < resolution.y; ++y) {
        for (int x = 0; x < resolution.x; ++x) {
            // Orientation
            cframe.orientation = this->getCFrame().orientation;
            // Position
            cframe.position = this->getCFrame().position;
            cframe.position.x += (x - resolution.x / 2);
            cframe.position.y += (y - resolution.y / 2);
            this->_rays[y * resolution.x + x]->setCFrame(cframe);
        }
    }
}
