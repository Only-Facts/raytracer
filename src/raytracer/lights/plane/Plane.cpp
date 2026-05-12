/**************************************************************\
Edition:
##  @date 02/05/2026 by @author Tsukini

File Name:
##  @file Plane.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/lights/Plane.hpp"
#include "raytracer/special/Utils.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"
#include <cmath>

void raytracer::Plane::parse(unused const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
    raytracer::ObjectDescriptor descriptor;

    // Setup the cframe
    raytracer::ObjectDescriptor::setCFrame(descriptor, node);

    // Other settings
    double intensity = 1.0;
    if (node.lookupValue("intensity", intensity))
        this->_intensity = intensity;

    double lumen = 1.0;
    if (node.lookupValue("lumen", lumen))
        this->_lumen = lumen;

    if (node.exists("color")) {
        const libconfig::Setting& color = node["color"];
        this->_color = {
            (int)color[0],
            (int)color[1],
            (int)color[2]
        };
    }

    // Set the descriptor
    this->setObjectDescriptor(descriptor);
}

void raytracer::Plane::init(void)
{
    std::size_t size = MAX_LIGHT_RAY;

    // Clear old data
    this->_rays.clear();

    // Resize screen size
    this->_rays.reserve(size);
    this->_rays.resize(size, nullptr);
    for (std::size_t i = 0; i < size; ++i)
        this->_rays[i] = new raytracer::LightRay();
}

void raytracer::Plane::reset(void)
{
    utils::vector::OVector2<int> resolution = {std::sqrt(MAX_LIGHT_RAY), std::sqrt(MAX_LIGHT_RAY)};
    raytracer::CFrame cframe = this->getCFrame();
    raytracer::Coord position = cframe.position;
    raytracer::Direction orientation = cframe.orientation;
    raytracer::Angle rotation = -cframe.rotation;
    raytracer::Coord2D rotated;

    // For each rays set default light value
    for (raytracer::LightRay* ray: this->_rays) {
        ray->reset();
        ray->setColor(this->_color);
        ray->setIntensity(this->_intensity);
        ray->setPower(this->_lumen);
    }

    // Pre compute x & y angles
    raytracer::Type angleY = raytracer::radToDeg(std::atan2(orientation.x, orientation.z));
    raytracer::Type angleX = raytracer::radToDeg(std::atan2(orientation.y, std::hypot(orientation.x, orientation.z)));
    utils::vector::OVector2<float> resolution2 = resolution / 2;

    // Set rays init position & orientation
    for (int y = 0; y < resolution.y; ++y) {
        for (int x = 0; x < resolution.x; ++x) {
            // Orientation (2D = same as the look vector)
            cframe.orientation = orientation;
            // Position
            cframe.position = position;
            cframe.position.x += (resolution2.x - x);
            cframe.position.y += (resolution2.y - y);
            // Apply rotation
            rotated = raytracer::rotatePoint2D({position.x, position.y}, {cframe.position.x, cframe.position.y}, rotation);
            cframe.position.x = rotated.x;
            cframe.position.y = rotated.y;
            // Apply look vector
            cframe.position = raytracer::rotatePoint3D(position, cframe.position, angleX, angleY);
            this->_rays[y * resolution.x + x]->setCFrame(cframe);
        }
    }
}
