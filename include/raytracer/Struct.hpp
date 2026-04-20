/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

File Name:
##  @file Struct.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef STRUCT_H
    #define STRUCT_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #include "utils/utils.hpp"          // utils::vector::Vector3, utils::vector::Vector2
    #include "materials/IMaterial.hpp"  // raytracer::IMaterial
    #include <cstddef>                  // std::size_t
    #include <memory>                   // std::shared_ptr

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

struct CFrame {
    utils::vector::Vector3<double> position = {0.0, 0.0, 0.0};
    utils::vector::Vector3<double> orientation = {0.0, 0.0, 0.0};
};

enum class Shape {
    None,
    Point,
    Plane,
    Sphere,
    Cylinder,
    Cone,
};

struct ShapeDescriptor {
    raytracer::Shape id = raytracer::Shape::None;

    /* all */
    raytracer::CFrame cframe;
    std::shared_ptr<raytracer::IMaterial> material;

    /* plane & cylinder & cone */
    utils::vector::Vector2<std::size_t> dimension = {0, 0}; // {width, height}

    /* sphere & cylinder & cone */
    float radius = 0.0f;

    // ---------- Constructor --------- //
    ShapeDescriptor() = default;
    ShapeDescriptor(const raytracer::CFrame& cframe): cframe{cframe} {};
};

} // namespace end
#endif /* STRUCT_H */
