/**************************************************************\
Edition:
##  @date 22/04/2026 by @author Tsukini

File Name:
##  @file AObject.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/objects/AObject.hpp"
#include "raytracer/rays/IRay.hpp"
#include "raytracer/Struct.hpp"
#include "raytracer/Define.hpp"
#include <algorithm>

void raytracer::AObject::reflectRay(std::shared_ptr<raytracer::IRay> ray) const
{
    raytracer::CFrame cframe = ray->getCFrame();
    utils::vector::Vector3<double> orientation = cframe.orientation.normalize();
    utils::vector::Vector3<double> hit = this->computeHit(cframe.position).normalize();
    if (orientation.dot(hit) > 0) hit = -hit;
    orientation = orientation - hit * (2.0 * orientation.dot(hit));
    cframe.orientation = orientation.normalize();
    ray->setCFrame(cframe);
}

#include <iostream>
utils::vector::Vector3<std::uint8_t> raytracer::AObject::getPointColor(const utils::vector::Vector3<double>& point) const
{
    utils::vector::Vector3<std::uint8_t> pointColor = this->getShapeDescriptor().material->getColor();
    bool found = false;

    // For each light rays
    for (const auto& [position, color, intensity]: this->_lightRays) {
        // Check if the point is near the light ray
        if ((point - position).length() > LIGHT_COLOR_LIMIT) continue;
        found = true;

        // Fuse the colors
        pointColor = std::clamp(
            static_cast<utils::vector::Vector3<float>>(pointColor).normalize() *
            static_cast<utils::vector::Vector3<float>>(color).normalize() *
            intensity,
            {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}
        ) * 255;
    }

    // No light ray on this hit
    if (!DEFAULT_LIGHT && !found)
        return DEFAULT_COLOR;

    return pointColor;
}
