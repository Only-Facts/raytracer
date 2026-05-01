/**************************************************************\
Edition:
##  @date 28/04/2026 by @author Tsukini

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

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* PROTOTYPE */

/* color */
inline nodiscard raytracer::Color mergeColor(raytracer::HugeColor color1, raytracer::HugeColor color2, float intensity)
{return color1 * color2 * intensity / 255;};

/* computing */
inline nodiscard raytracer::Type degToRad(raytracer::Angle deg)
{return deg * M_PI / 180.0;};
inline nodiscard raytracer::Angle radToDeg(raytracer::Type rad)
{return rad / M_PI * 180.0;};
raytracer::Coord2D rotatePoint2D(const raytracer::Coord2D& origin, const raytracer::Coord2D& point, raytracer::Angle angle);
raytracer::Coord rotatePoint3D(const raytracer::Coord& origin, const raytracer::Coord& point, raytracer::Angle angleX, raytracer::Angle angleY);

} // namespace end
#endif /* UTILS_H */
