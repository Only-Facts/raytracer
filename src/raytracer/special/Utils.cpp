/**************************************************************\
Edition:
##  @date 30/04/2026 by @author Tsukini

File Name:
##  @file Utils.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/special/Utils.hpp"
#include <cmath>

nodiscard raytracer::Coord2D raytracer::rotatePoint2D(const raytracer::Coord2D& origin, const raytracer::Coord2D& point, raytracer::Angle angle)
{
    raytracer::Type rad = raytracer::degToRad(angle);

    // Remove the origin
    raytracer::Type x = point.x - origin.x;
    raytracer::Type y = point.y - origin.y;

    // Rotation
    raytracer::Type cosR = std::cos(rad);
    raytracer::Type sinR = std::sin(rad);
    raytracer::Type xr = x * cosR - y * sinR;
    raytracer::Type yr = x * sinR + y * cosR;

    // Re apply the origin
    return {xr + origin.x, yr + origin.y};
}

nodiscard raytracer::Coord raytracer::rotatePoint3D(const raytracer::Coord& origin, const raytracer::Coord& point, raytracer::Angle angleX, raytracer::Angle angleY)
{
    // Convert to rad
    raytracer::Type radX = raytracer::degToRad(angleX);
    raytracer::Type radY = raytracer::degToRad(angleY);

    // Remove the origin
    raytracer::Type x = point.x - origin.x;
    raytracer::Type y = point.y - origin.y;
    raytracer::Type z = point.z - origin.z;

    // Rotation x
    raytracer::Type cosX = std::cos(radX);
    raytracer::Type sinX = std::sin(radX);
    raytracer::Type x1 = x * cosX - z * sinX;
    raytracer::Type z1 = x * sinX + z * cosX;
    raytracer::Type y1 = y;

    // Rotation y
    raytracer::Type cosY = std::cos(radY);
    raytracer::Type sinY = std::sin(radY);
    raytracer::Type y2 = y1 * cosY - z1 * sinY;
    raytracer::Type z2 = y1 * sinY + z1 * cosY;
    raytracer::Type x2 = x1;

    // Re apply the origin
    return {x2 + origin.x, y2 + origin.y, z2 + origin.z};
}
