/**************************************************************\
Edition:
##  @date 30/04/2026 by @author Tsukini

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
    #include "utils/utils.hpp"  // utils::exception::*, utils::vector::*, hot, nodiscard
    #include "Define.hpp"       // CHUNK_SIZE
    #include <libconfig.h++>    // libconfig::Setting
    #include <cstddef>          // std::size_t
    #include <cstdint>          // std::uint8_t, std::uint16_t, std::int32_t
    #include <vector>           // std::vector
    #include <tuple>            // std::tuple

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS & TYPEDEF */

using Type = double; // Type used everywhere for coord, angle, direction, computing, ...

using Coord2D = utils::vector::OVector2<raytracer::Type>;
using Coord = utils::vector::OVector3<raytracer::Type>;
using Direction = utils::vector::OVector3<raytracer::Type>; // Generaly normalized
using Angle = raytracer::Type;

using Chunk = std::tuple<std::int32_t, std::int32_t, std::int32_t>;

using Resolution = utils::vector::OVector2<std::uint16_t>;

using Color = utils::vector::OVector3<std::uint8_t>;
using HugeColor = utils::vector::OVector3<std::uint16_t>;

using Vertice = utils::vector::OVector3<raytracer::Type>;
using Face = std::vector<Vertice>;

struct CFrame {
    raytracer::Coord position = {0.0, 0.0, 0.0};
    raytracer::Direction orientation = {0.0, 0.0, 0.0};
    raytracer::Angle rotation = 0.0;
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
    raytracer::CFrame cframeOrigin;
    raytracer::IMaterial* material;
    std::vector<raytracer::Face> faces;

    /* cone & cylinder & plane & sphere */
    float scale = 1.0f;

    // ---------- Constructor --------- //
    ObjectDescriptor() = default;
    ObjectDescriptor(const raytracer::CFrame& cframe): cframe{cframe}, cframeOrigin{cframe} {};

    // -------- Static-Function ------- //
    static void setCFrame(raytracer::ObjectDescriptor& descriptor, const libconfig::Setting& node)
    {
        // Check existantce
        if (!node.exists("cframe"))
            throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The CFrame field isn't defined");
        const libconfig::Setting& cframe = node["cframe"];
        if (!cframe.exists("position"))
            throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The position field isn't defined for the CFrame");
        else if (!cframe.exists("orientation"))
            throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "The orientation field isn't defined for the CFrame");

        // Set values
        const libconfig::Setting& pos = cframe["position"];
        descriptor.cframe.position.x = pos[0];
        descriptor.cframe.position.y = pos[1];
        descriptor.cframe.position.z = pos[2];
        const libconfig::Setting& rot = cframe["orientation"];
        descriptor.cframe.orientation.x = rot[0];
        descriptor.cframe.orientation.y = rot[1];
        descriptor.cframe.orientation.z = rot[2];
        double rotation = 0.0;
        if (cframe.lookupValue("rotation", rotation))
            descriptor.cframe.rotation = rotation;

        // Normalize orientation (only if not null)
        if (descriptor.cframe.orientation.x >= 1e-8 || descriptor.cframe.orientation.y >= 1e-8 || descriptor.cframe.orientation.z >= 1e-8
            || descriptor.cframe.orientation.x <= -1e-8 || descriptor.cframe.orientation.y <= -1e-8 || descriptor.cframe.orientation.z <= -1e-8)
            descriptor.cframe.orientation = descriptor.cframe.orientation.normalize();

        descriptor.cframeOrigin = descriptor.cframe;
    }
};

} // namespace end
#endif /* STRUCT_H */
