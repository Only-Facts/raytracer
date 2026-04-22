/**************************************************************\
Edition:
##  @date 22/04/2026 by @author Tsukini

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
#include "raytracer/rays/Ray.hpp"
#include <limits>
#include <cmath>

void raytracer::Camera::parse(unused const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ShapeDescriptor descriptor;

    // Setup the cframe
    raytracer::Raytracer::setCFrame(descriptor, node);

    // Other settings
    if (!node.exists("resolution"))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The resolution field isn't defined for the camera");
    const libconfig::Setting& res = node["resolution"];
    descriptor.dimension = {(int)res[0], (int)res[1]};

    double fieldOfView = 70.0f;
    if (node.lookupValue("fieldOfView", fieldOfView))
        this->_fieldOfView = fieldOfView;

    // Set the descriptor
    this->setShapeDescriptor(descriptor);
}

void raytracer::Camera::init(void)
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
        this->_rays[i] = std::make_shared<raytracer::Ray>();
}

void raytracer::Camera::reset(void)
{
    utils::vector::Vector2<std::uint16_t> resolution;
    raytracer::CFrame cframe;
    double aspect, scale;
    double px, py, u, v;

    // For each rays set default light value
    for (std::shared_ptr<raytracer::Ray>& ray: this->_rays)
        ray->reset();

    // Set rays init position & orientation
    resolution = this->getResolution();
    aspect = resolution.x / resolution.y;
    scale = std::tan(this->_fieldOfView * 0.5 * M_PI / 180.0);
    for (std::size_t y = 0; y < resolution.y; ++y) {
        for (std::size_t x = 0; x < resolution.x; ++x) {
            // Orientation
            u = (x + 0.5) / resolution.x;
            v = (y + 0.5) / resolution.y;
            px = (2.0 * u - 1.0) * aspect * scale;
            py = (1.0 - 2.0 * v) * scale;
            cframe.orientation = {px, py, -1.0};
            cframe.orientation = cframe.orientation.normalize();
            // Position
            cframe.orientation = this->getCFrame().orientation;
            cframe.position = this->getCFrame().position;
            cframe.position.x += x;
            cframe.position.y += y;
            this->_rays[y * resolution.x + x]->setCFrame(cframe);
        }
    }
}
