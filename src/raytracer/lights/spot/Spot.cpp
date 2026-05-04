/**************************************************************\
Edition:
##  @date 02/05/2026 by @author Tsukini

File Name:
##  @file Spot.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/lights/Spot.hpp"
#include "raytracer/special/Utils.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"
#include <cmath>

void raytracer::Spot::parse(unused const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
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

    double fieldOfLight = 70.0f;
    if (node.lookupValue("fieldOfLight", fieldOfLight))
        this->_fieldOfLight = fieldOfLight;

    // Set the descriptor
    this->setObjectDescriptor(descriptor);
}

void raytracer::Spot::init(void)
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

void raytracer::Spot::reset(void)
{
    utils::vector::OVector2<int> resolution = {std::sqrt(MAX_LIGHT_RAY), std::sqrt(MAX_LIGHT_RAY)};
    raytracer::CFrame cframe = this->getCFrame();
    raytracer::Coord position = cframe.position;
    raytracer::Direction orientation = cframe.orientation;
    raytracer::Angle rotation = cframe.rotation;
    raytracer::Angle angleX = 0.0, angleY = 0.0;
    raytracer::Coord2D rotated;

    // For each rays set default light value
    for (raytracer::LightRay* ray: this->_rays) {
        ray->reset();
        ray->setColor(this->_color);
        ray->setIntensity(this->_intensity);
        ray->setPower(this->_lumen);
    }

    // Pre compute values
    float fieldOfLightW = this->_fieldOfLight * .5;
    float fieldOfLightH = (resolution.y / static_cast<float>(resolution.x)) * this->_fieldOfLight * .5;
    raytracer::Type diffAngleY = raytracer::radToDeg(std::atan2(orientation.x, orientation.z));
    raytracer::Type diffAngleX = raytracer::radToDeg(std::atan2(orientation.y, std::hypot(orientation.x, orientation.z)));
    utils::vector::OVector2<float> resolution2 = resolution / 2;

    // Set rays init position & orientation
    for (int y = 0; y < resolution.y; ++y) {
        angleX = ((y - resolution2.y) / resolution2.y) * fieldOfLightH;
        for (int x = 0; x < resolution.x; ++x) {
            angleY = ((x - resolution2.x) / resolution2.x) * fieldOfLightW;
            // Orientation
            cframe.orientation = orientation;
            // Apply rotation
            cframe.orientation = raytracer::rotatePoint3D({0.0, 0.0, 0.0}, cframe.orientation, -diffAngleX, -diffAngleY);
            rotated = raytracer::rotatePoint2D({0.0, 0.0}, {cframe.orientation.x, cframe.orientation.y}, rotation);
            cframe.orientation.x = rotated.x;
            cframe.orientation.y = rotated.y;
            cframe.orientation = raytracer::rotatePoint3D({0.0, 0.0, 0.0}, cframe.orientation, diffAngleX, diffAngleY);
            // Apply field of view
            cframe.orientation = raytracer::rotatePoint3D({0.0, 0.0, 0.0}, cframe.orientation, angleX, angleY);
            // Position (FOV = same as the origin)
            cframe.position = position;
            this->_rays[y * resolution.x + x]->setCFrame(cframe);
        }
    }
}
