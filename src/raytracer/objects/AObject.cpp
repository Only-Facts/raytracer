/**************************************************************\
Edition:
##  @date 04/05/2026 by @author Tsukini

File Name:
##  @file AObject.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Attribute
#define _Exception
#include "utils/utils.hpp"
#include "raytracer/special/Utils.hpp"
#include "raytracer/objects/AObject.hpp"
#include "raytracer/rays/IRay.hpp"
#include "raytracer/Raytracer.hpp"
#include "raytracer/Struct.hpp"
#include "raytracer/Define.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <algorithm>
#include <exception>
#include <iostream>

static void processChunk(const std::vector<raytracer::ChunkLightData>& data,
    const raytracer::Coord& point, raytracer::FColor& lightColor,
    float& count)
{
    static raytracer::Type limit = LIGHT_COLOR_LIMIT * LIGHT_COLOR_LIMIT;
    const raytracer::Type px = point.x;
    const raytracer::Type py = point.y;
    const raytracer::Type pz = point.z;

    for (const raytracer::ChunkLightData& s: data) {
        // Check if the point is near the light ray
        const raytracer::Type dx = px - s.position.x;
        const raytracer::Type dy = py - s.position.y;
        const raytracer::Type dz = pz - s.position.z;
        const raytracer::Type d2 = dx * dx + dy * dy + dz * dz;
        if (d2 > limit) continue;

        // Compute the coef
        //float proximityCoef = 1.0f - (d2 / limit);

        // Fuse the colors
        lightColor.x += s.color.x * s.intensity;// * proximityCoef;
        lightColor.y += s.color.y * s.intensity;// * proximityCoef;
        lightColor.z += s.color.z * s.intensity;// * proximityCoef;
        ++count;
    }
}

hot std::pair<raytracer::Color, bool> raytracer::AObject::getPointColor(const raytracer::Coord& point) const
{
    raytracer::FColor lightColor = DEFAULT_COLOR;
    raytracer::Coord chunkPoint;
    raytracer::Chunk chunk;
    float count = 0;

    // For each chunk around the chunk point
    static int limits = std::ceil(CHUNK_SIZE / LIGHT_COLOR_LIMIT);
    for (int z = -limits; z <= limits; ++z)
    for (int y = -limits; y <= limits; ++y)
    for (int x = -limits; x <= limits; ++x) {
        chunkPoint = {x * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE};
        chunk = raytracer::getChunk(point + chunkPoint);
        auto it = this->_lightData.find(chunk);
        if (it == this->_lightData.end()) continue;
        processChunk(it->second, point, lightColor, count);
    }

    // No light ray on this hit
    if (!DEFAULT_LIGHT && !count)
        return {DEFAULT_COLOR, false};

    // Apply the light modifier
    raytracer::FColor color = this->getObjectDescriptor().material->getColor();
    if (color.x < 1e-8) color.x = NO_LIGHT_DEFAULT;
    if (color.y < 1e-8) color.y = NO_LIGHT_DEFAULT;
    if (color.z < 1e-8) color.z = NO_LIGHT_DEFAULT;
    color *= lightColor / (count * 255.0f);
    return {{std::min(color.x, 255.0f), std::min(color.y, 255.0f), std::min(color.z, 255.0f)}, true};
}

hot void raytracer::AObject::addLightData(raytracer::Coord position, raytracer::Color color, float intensity)
{
    raytracer::Chunk chunk = raytracer::getChunk(position);
    std::lock_guard<std::mutex> lock(this->_lock);
    this->_lightData[chunk].push_back({position, color, intensity});
}

cold void raytracer::AObject::loadObj(const std::string& path, raytracer::ObjectDescriptor& descriptor)
{
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig config;
    raytracer::Chunk chunkMin, chunkMax;
    raytracer::Chunk chunk;
    raytracer::Vertice verticeMin, verticeMax;
    raytracer::Vertice vertice;
    config.triangulate = true;

    // Check error & warning
    if (!reader.ParseFromFile(path, config)) {
        std::string err = reader.Error();
        err.pop_back();
        utils::exception::CustomException e(utils::exception::Error, utils::exception::Code::Parser, err);
        std::cout << e.formated() << std::endl;
    }
    if (!reader.Warning().empty()) {
        utils::exception::CustomException e(utils::exception::Warning, utils::exception::Code::Parser, reader.Warning());
        std::cout << e.formated() << std::endl;
    }

    // Compute world rotation
    raytracer::Coord orientation = descriptor.cframe.orientation;
    raytracer::Type len = orientation.dot(orientation);
    if (len < 1e-12) orientation = {0, 0, 1}; // Fallback orientation
    raytracer::Coord forward = orientation;
    raytracer::Coord worldUp = {0, 1, 0};
    if (std::abs(forward.dot(worldUp)) > 0.999) worldUp = {1, 0, 0}; // Edge case, parrallel
    raytracer::Coord right = (worldUp.cross(forward)).normalize();
    raytracer::Coord up = forward.cross(right);

    // Get the file content
    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    // For each shape get the different vertex
    for (const auto& shape: shapes) {
        // For each face get the vertices (only accept face of 3 or less vertices)
        for (std::size_t f = 0, indexOffset = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            // Check the number of vertices
            std::size_t verticesCount = shape.mesh.num_face_vertices[f];
            if (verticesCount < MAX_VERTICES)
                throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, std::string("Invalid vertices number, sould be inferior or equal to '") + std::to_string(MAX_VERTICES) + "', but got: " + std::to_string(f));

            // Setup the vertex
            raytracer::Face face;
            for (std::size_t v = 0; v < verticesCount; ++v) {
                const tinyobj::index_t& idx = shape.mesh.indices[indexOffset + v];

                // Get the vertice
                vertice.x = attrib.vertices[3 * idx.vertex_index + 0];
                vertice.y = attrib.vertices[3 * idx.vertex_index + 1];
                vertice.z = attrib.vertices[3 * idx.vertex_index + 2];

                // Apply rotation & offset
                raytracer::Coord rotated = right * vertice.x + up * vertice.y + forward * vertice.z;
                rotated *= descriptor.scale;
                vertice.x = rotated.x + descriptor.cframe.position.x;
                vertice.y = rotated.y + descriptor.cframe.position.y;
                vertice.z = rotated.z + descriptor.cframe.position.z;

                // Min & Max
                if (v == 0) {
                    verticeMin = vertice;
                    verticeMax = vertice;
                } else {
                    verticeMin.x = std::min(verticeMin.x, vertice.x);
                    verticeMin.y = std::min(verticeMin.y, vertice.y);
                    verticeMin.z = std::min(verticeMin.z, vertice.z);
                    verticeMax.x = std::max(verticeMax.x, vertice.x);
                    verticeMax.y = std::max(verticeMax.y, vertice.y);
                    verticeMax.z = std::max(verticeMax.z, vertice.z);
                }

                // Store the vertice
                face.push_back(vertice);
            }

            // Store the face
            chunkMin = raytracer::getChunk(verticeMin);
            chunkMax = raytracer::getChunk(verticeMax);
            for (int z = chunkMin.z; z <= chunkMax.z; ++z)
            for (int y = chunkMin.y; y <= chunkMax.y; ++y)
            for (int x = chunkMin.x; x <= chunkMax.x; ++x) {
                chunk = {x, y, z};
                descriptor.chunks.push_back(chunk);
            }
            descriptor.faces.push_back(face);
            indexOffset += verticesCount;
        }
    }
}

hot void raytracer::AObject::reflectRay(raytracer::IRay* ray, const raytracer::Face* face) const
{
    raytracer::CFrame cframe = ray->getCFrame();
    raytracer::Coord orientation = cframe.orientation;
    raytracer::Coord hit = this->computeHit(cframe.position, face).normalize();
    if (orientation.dot(hit) > 0) hit = -hit;
    orientation = orientation - hit * (2.0 * orientation.dot(hit));
    cframe.orientation = orientation;
    ray->setCFrame(cframe);
}

static float segmentSDF(const raytracer::Coord& point, const raytracer::Vertice& a, const raytracer::Vertice& b)
{
    raytracer::Coord ab = b - a;
    raytracer::Type v = ab.dot(ab);
    if (v == 0.0) return (point - a).lengthSquared(); // Security
    raytracer::Type t = (point - a).dot(ab) / v;
    t = std::clamp(t, 0.0, 1.0);
    raytracer::Coord closest = a + t * ab;
    return (point - closest).lengthSquared();
}

/*
static float triangleSDF(const raytracer::Coord& point, const raytracer::Vertice& a, const raytracer::Vertice& b, const raytracer::Vertice& c)
{
    raytracer::Coord ba = b - a, pa = point - a;
    raytracer::Coord cb = c - b, pb = point - b;
    raytracer::Coord ac = a - c, pc = point - c;
    raytracer::Coord nor = ba.cross(ac);

    return std::sqrt(
        (sign(ba.cross(nor).dot(pa)) +
         sign(cb.cross(nor).dot(pb)) +
         sign(ac.cross(nor).dot(pc)) < 2.0)
        ?
        std::min(
            std::min(
                (ba * std::clamp(ba.dot(pa) / ba.lengthSquared(), 0.0, 1.0)).lengthSquared()
                ,
                (cb * std::clamp(cb.dot(pb) / cb.lengthSquared(), 0.0, 1.0)).lengthSquared()
            )
            ,
            (ac * std::clamp(ac.dot(pc) / ac.lengthSquared(), 0.0, 1.0)).lengthSquared()
        )
        :
        nor.dot(pa) * nor.dot(pa) / nor.lengthSquared()
    );
}
*/

hot static float triangleSDF(const raytracer::Coord& point, const raytracer::Vertice& a, const raytracer::Vertice& b, const raytracer::Vertice& c)
{
    raytracer::Coord ab = b - a;
    raytracer::Coord ac = c - a;
    raytracer::Coord ap = point - a;

    // normale du triangle
    raytracer::Coord n = ab.cross(ac);
    raytracer::Type nLen2 = n.dot(n);

    // Invalid triangle
    if (nLen2 == 0.0) {
        return std::min({
            segmentSDF(point, a, b),
            segmentSDF(point, b, c),
            segmentSDF(point, c, a)
        });
    }

    // Project the point on the triangle
    raytracer::Type distPlane = ap.dot(n);
    raytracer::Coord proj = point - n * (distPlane / nLen2);

    // Check if the point is inside
    raytracer::Coord bp = proj - b;
    raytracer::Coord cp = proj - c;

    raytracer::Coord bc = c - b;
    raytracer::Coord ca = a - c;

    // Edge case, same orientation
    if ((ab.cross(proj - a)).dot(n) >= 0
        && (bc.cross(bp)).dot(n) >= 0
        && (ca.cross(cp)).dot(n) >= 0)
        return (distPlane * distPlane) / nLen2;

    // Edge case, on side
    return std::min({
        segmentSDF(point, a, b),
        segmentSDF(point, b, c),
        segmentSDF(point, c, a)
    });
}

hot std::pair<float, const raytracer::Face*> raytracer::AObject::computeSDF(const raytracer::Coord& point) const
{
    float sdf = std::numeric_limits<float>::max(), dist = 0.0f;
    const raytracer::Face* sdfFace = nullptr;

    // For each face
    for (const raytracer::Face& face: this->getObjectDescriptor().faces) {
        // Dispatch the computing
        switch (face.size()) {
            case 1: dist = (point - face[0]).lengthSquared();               break;
            case 2: dist = segmentSDF(point, face[0], face[1]);             break;
            case 3: dist = triangleSDF(point, face[0], face[1], face[2]);   break;
            default:
                throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "Invalid number of vertices for a face on the object to render");
        }

        // Check the distance
        if (dist < sdf) {
            sdf = dist;
            sdfFace = &face;
        }
    }

    return {std::sqrt(sdf), sdfFace};
}

hot static raytracer::Coord segmentHit(const raytracer::Coord& point, const raytracer::Vertice& a, const raytracer::Vertice& b)
{
    raytracer::Coord ab = b - a;
    raytracer::Coord ap = point - a;
    raytracer::Coord proj = a + ab * (ap.dot(ab) / ab.lengthSquared());
    return (point - proj).normalize();
}

hot static raytracer::Coord triangleHit(const raytracer::Vertice& a, const raytracer::Vertice& b, const raytracer::Vertice& c)
{
    raytracer::Coord n = (b - a).cross(c - a);
    return n.normalize();
}

hot raytracer::Direction raytracer::AObject::computeHit(const raytracer::Coord& point, const raytracer::Face* facePtr) const
{
    // Check if the sdf was already computed
    if (!facePtr) unlikely {
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::InvalidAction, "Can't compute the perpendicular vector for the hit point before the sdf");
    }

    // Dispatch the computing
    const raytracer::Face face = *(facePtr);
    switch (face.size()) {
        case 1: return (point - face[0]).normalize();
        case 2: return segmentHit(point, face[0], face[1]);
        case 3: return triangleHit(face[0], face[1], face[2]);
        default:
            throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "Invalid number of vertices for a face on the object to render");
    }
}
