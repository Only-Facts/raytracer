/**************************************************************\
Edition:
##  @date 30/04/2026 by @author Tsukini

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
#include "raytracer/special/Utils.hpp"
#include "raytracer/Struct.hpp"
#include "raytracer/Define.hpp"
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
    
    double renderDistance = RENDER_DISTANCE;
    if (node.lookupValue("renderDistance", renderDistance))
        this->_renderDistance = renderDistance;

    // Set the descriptor
    this->setObjectDescriptor(descriptor);
}

void raytracer::CameraFOV::init(void)
{
    std::size_t size = static_cast<std::size_t>(this->getResolution().x) * this->getResolution().y;

    // Clear old data
    this->_screen.clear();
    for (std::size_t i = 0; i < this->_rays.size(); ++i)
        delete this->_rays[i];
    this->_rays.clear();

    // Resize screen size
    this->_screen.reserve(size);
    this->_rays.reserve(size);
    this->_screen.resize(size, DEFAULT_COLOR);
    this->_rays.resize(size, nullptr);
    for (std::size_t i = 0; i < size; ++i)
        this->_rays[i] = new raytracer::Ray();
}

void raytracer::CameraFOV::reset(void)
{
    
    utils::vector::OVector2<int> resolution = this->getResolution();
    raytracer::CFrame cframe = this->getCFrame();
    raytracer::Coord position = cframe.position;
    raytracer::Direction orientation = cframe.orientation;
    raytracer::Angle rotation = cframe.rotation;
    raytracer::Angle angleX = 0.0, angleY = 0.0;
    raytracer::Coord2D rotated;

    // For each rays set default light value
    for (raytracer::Ray* ray: this->_rays)
        ray->reset();

    // Pre compute values
    float fieldOfViewW = this->_fieldOfView * .5;
    float fieldOfViewH = (resolution.y / static_cast<float>(resolution.x)) * this->_fieldOfView * .5;
    raytracer::Type diffAngleY = raytracer::radToDeg(std::atan2(orientation.x, orientation.z));
    raytracer::Type diffAngleX = raytracer::radToDeg(std::atan2(orientation.y, std::hypot(orientation.x, orientation.z)));
    utils::vector::OVector2<float> resolution2 = resolution / 2;

    // Set rays init position & orientation
    for (int y = 0; y < resolution.y; ++y) {
        angleX = ((y - resolution2.y) / resolution2.y) * fieldOfViewH;
        for (int x = 0; x < resolution.x; ++x) {
            angleY = ((x - resolution2.x) / resolution2.x) * fieldOfViewW;
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
