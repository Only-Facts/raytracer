/**************************************************************\
Edition:
##  @date 27/04/2026 by @author Tsukini

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
    #define _Attribute
    #include "utils/utils.hpp"  // utils::exception::*, utils::vector::Vector3, utils::vector::Vector2, hot, nodiscard
    #include "Define.hpp"       // CHUNK_SIZE
    #include <libconfig.h++>    // libconfig::Setting
    #include <cstddef>          // std::size_t
    #include <cstdint>          // std::uint8_t, std::uint16_t, std::int32_t
    #include <vector>           // std::vector
    #include <tuple>            // std::tuple

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS & TYPEDEF */

using Coord = utils::vector::Vector3<double>;

using Chunk = std::tuple<std::int32_t, std::int32_t, std::int32_t>;

using Color = utils::vector::Vector3<std::uint8_t>;
using HugeColor = utils::vector::Vector3<std::uint16_t>;

using Vertice = utils::vector::Vector3<double>;
using Face = std::vector<Vertice>;

struct CFrame {
    raytracer::Coord position = {0.0, 0.0, 0.0};
    raytracer::Coord orientation = {0.0, 0.0, 0.0};
};

enum class Shape {
    None,
    Point,
    Plane,
    Sphere,
    Cylinder,
    Cone,
};

inline hot nodiscard raytracer::Chunk getChunk(const raytracer::Coord& point)
{
    return {
        point.x / CHUNK_SIZE,
        point.y / CHUNK_SIZE,
        point.z / CHUNK_SIZE
    };
}

struct ChunkHash {
    std::size_t operator()(const raytracer::Chunk& chunk) const noexcept {
        auto [x, y, z] = chunk;
        return (x * 73856093) ^ (y * 19349663) ^ (z * 83492791);
    }
};

struct ChunkObjectData {
    std::vector<raytracer::Coord> positions;
    std::vector<raytracer::Color> colors;
    std::vector<float> intensitys;
};

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
