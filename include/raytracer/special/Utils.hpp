/**************************************************************\
Edition:
##  @date 01/05/2026 by @author Tsukini

File Name:
##  @file Utils.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef UTILS_H
    #define UTILS_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Attribute
    #include "utils/utils.hpp"  // nodiscard
    #include "../Struct.hpp"    // raytracer::* (types)
    #include <cstdint>          // std::uint16_t

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* PROTOTYPE */

/* color */
void noise(const raytracer::Coord& p, raytracer::FColor& v, float strength = NOISE_STRENGTH, float size = NOISE_SIZE);
inline nodiscard raytracer::Color moyColor(raytracer::HugeColor color1, raytracer::HugeColor color2, float intensity = 1.0)
{return (color1 + color2 * intensity) / (1 + intensity);};
inline nodiscard raytracer::Color mergeColor(raytracer::HugeColor color1, raytracer::HugeColor color2, float intensity = 1.0)
{
    /*
    if (color1 == 0 && color2 != 0) return color2;
    else if (color2 == 0 && color1 != 0) return color1;
    else if (color1 == 0 && color2 == 0) return {0, 0, 0};
    else return color1 * color2 * intensity / 255;
    */
    //return (color1 + color2 * intensity / 255;
    //return (color1 + color2 * intensity) / (1 + intensity);
    raytracer::HugeColor newColor = color1 + (color2 * intensity);
    return {
        std::min(newColor.x, (std::uint16_t)255),
        std::min(newColor.y, (std::uint16_t)255),
        std::min(newColor.z, (std::uint16_t)255)
    };
};

/* computing */
inline nodiscard raytracer::Type degToRad(raytracer::Angle deg)
{return deg * M_PI / 180.0;};
inline nodiscard raytracer::Angle radToDeg(raytracer::Type rad)
{return rad / M_PI * 180.0;};
raytracer::Coord2D rotatePoint2D(const raytracer::Coord2D& origin, const raytracer::Coord2D& point, raytracer::Angle angle);
raytracer::Coord rotatePoint3D(const raytracer::Coord& origin, const raytracer::Coord& point, raytracer::Angle angleX, raytracer::Angle angleY);

} // namespace end
#endif /* UTILS_H */
