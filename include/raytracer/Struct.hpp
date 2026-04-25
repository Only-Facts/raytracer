/**************************************************************\
Edition:
##  @date 25/04/2026 by @author Tsukini

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
    #define _Exception
    #include "utils/utils.hpp"  // utils::exception::*, utils::vector::Vector3, utils::vector::Vector2
    #include <libconfig.h++>    // libconfig::Setting
    #include <cstddef>          // std::size_t
    #include <cstdint>          // std::uint8_t
    #include <memory>           // std::shared_ptr
    #include <vector>           // std::vector

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

using Color = utils::vector::Vector3<std::uint8_t>;
using HugeColor = utils::vector::Vector3<std::uint16_t>;

using Vertice = utils::vector::Vector3<double>;
using Face = std::vector<Vertice>;

class IMaterial;

struct ObjectDescriptor {
    raytracer::Shape id = raytracer::Shape::None;

    /* all */
    raytracer::CFrame cframe;
    raytracer::IMaterial* material;
    std::vector<raytracer::Face> faces;

    /* cone & cylinder & plane & sphere */
    float scale = 1.0f;

    // ---------- Constructor --------- //
    ObjectDescriptor() = default;
    ObjectDescriptor(const raytracer::CFrame& cframe): cframe{cframe} {};

    // -------- Static-Function ------- //
    static void setCFrame(raytracer::ObjectDescriptor& descriptor, const libconfig::Setting& node)
    {
        // Check existantce
        if (!node.exists("position"))
            throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The position field isn't defined for the CFrame");
        else if (!node.exists("orientation"))
            throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The orientation field isn't defined for the CFrame");

        // Set values
        const libconfig::Setting& pos = node["position"];
        descriptor.cframe.position.x = pos[0];
        descriptor.cframe.position.y = pos[1];
        descriptor.cframe.position.z = pos[2];
        const libconfig::Setting& rot = node["orientation"];
        descriptor.cframe.orientation.x = rot[0];
        descriptor.cframe.orientation.y = rot[1];
        descriptor.cframe.orientation.z = rot[2];

        // Normalize orientation (only if not null)
        if (descriptor.cframe.orientation >= 1e-8 || descriptor.cframe.orientation <= -1e-8)
            descriptor.cframe.orientation = descriptor.cframe.orientation.normalize();
    }
};

} // namespace end
#endif /* STRUCT_H */
