/**************************************************************\
Edition:
##  @date 16/05/2026 by @author Tsukini

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
    raytracer::Chunk chunk;
    float count = 0;

    // Get the min & max chunk
    raytracer::Coord minPoint = {
        point.x - LIGHT_COLOR_LIMIT,
        point.y - LIGHT_COLOR_LIMIT,
        point.z - LIGHT_COLOR_LIMIT
    };
    raytracer::Coord maxPoint = {
        point.x + LIGHT_COLOR_LIMIT,
        point.y + LIGHT_COLOR_LIMIT,
        point.z + LIGHT_COLOR_LIMIT
    };
    raytracer::Chunk minChunk = raytracer::getColorChunk(minPoint);
    raytracer::Chunk maxChunk = raytracer::getColorChunk(maxPoint);

    // For each chunk around the chunk point
    for (int z = minChunk.z; z <= maxChunk.z; ++z)
    for (int y = minChunk.y; y <= maxChunk.y; ++y)
    for (int x = minChunk.x; x <= maxChunk.x; ++x) {
        chunk = {x, y, z};
        auto it = this->_lightData.find(chunk);
        if (it == this->_lightData.end()) continue;
        processChunk(it->second, point, lightColor, count);
    }

    // No light ray on this hit
    if (!DEFAULT_LIGHT && !count)
        return {DEFAULT_COLOR, false};

    // Apply the light modifier
    return {raytracer::mergeLight(this->getObjectDescriptor().material->getColor(), lightColor, count), true};
}

hot void raytracer::AObject::addLightData(raytracer::Coord position, raytracer::Color color, float intensity)
{
    raytracer::Chunk chunk = raytracer::getColorChunk(position);
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
 
                /* First try of rotation
                raytracer::Coord2D rotated = raytracer::rotatePoint2D({0.0, 0.0}, {vertice.x, vertice.y}, descriptor.cframe.orientation.z);
                vertice.x = rotated.x;
                vertice.y = rotated.y;
                vertice = raytracer::rotatePoint3D({0.0, 0.0, 0.0}, vertice, descriptor.cframe.);
                */

                // Compute direction vectors
                raytracer::Direction forward = descriptor.cframe.look;
                raytracer::Direction worldUp = {0, 1, 0};
                raytracer::Direction right = worldUp.cross(forward).normalize();
                raytracer::Direction up = forward.cross(right);

                // Roll
                float roll = raytracer::degToRad(descriptor.cframe.orientation.z);
                float c = std::cos(roll);
                float s = std::sin(roll);
                raytracer::Direction rolledRight = right * c + up * s;
                raytracer::Direction rolledUp = -right * s + up * c;

                // Apply scale & rotation & offset
                vertice *= descriptor.scale;
                vertice = rolledRight * vertice.x + rolledUp * vertice.y + forward * vertice.z;
                vertice += descriptor.cframe.position;

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

            // Store the face by chunck
            chunkMin = raytracer::getSpaceChunk(verticeMin);
            chunkMax = raytracer::getSpaceChunk(verticeMax);
            for (int z = chunkMin.z; z <= chunkMax.z; ++z)
            for (int y = chunkMin.y; y <= chunkMax.y; ++y)
            for (int x = chunkMin.x; x <= chunkMax.x; ++x) {
                chunk = {x, y, z};
                descriptor.faces[chunk].push_back(face);
            }
            descriptor.fallbackFaces.push_back(face);
            indexOffset += verticesCount;
        }
    }
}

hot void raytracer::AObject::reflectRay(raytracer::IRay* ray, const raytracer::Face* face) const
{
    raytracer::CFrame cframe = ray->getCFrame();
    raytracer::Direction look = cframe.look;
    raytracer::Coord hit = this->computeHit(cframe.position, face).normalize();
    if (look.dot(hit) > 0) hit = -hit;
    look = look - hit * (2.0 * look.dot(hit));
    cframe.look = look;
    ray->setCFrame(cframe, false);
}

hot static float segmentSDF(const raytracer::Coord& p, const raytracer::Vertice& a, const raytracer::Vertice& b)
{
    raytracer::Coord pa = p - a, ba = b - a;
    raytracer::Type h = pa.dot(ba) / ba.dot(ba);
    h = (h < static_cast<raytracer::Type>(0)) ? static_cast<raytracer::Type>(0) : (h > static_cast<raytracer::Type>(1) ? static_cast<raytracer::Type>(1) : h);
    return (pa - ba * h).lengthSquared();
}

/*
hot static float triangleSDF(const raytracer::Coord& p, const raytracer::Vertice& a, const raytracer::Vertice& b, const raytracer::Vertice& c)
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
*/

hot static float triangleSDF(const raytracer::Coord& p, const raytracer::Vertice& a, const raytracer::Vertice& b, const raytracer::Vertice& c)
{
    // edges
    const float abx = b.x - a.x;
    const float aby = b.y - a.y;
    const float abz = b.z - a.z;

    const float acx = c.x - a.x;
    const float acy = c.y - a.y;
    const float acz = c.z - a.z;

    // normal = ab x ac
    const float nx = aby*acz - abz*acy;
    const float ny = abz*acx - abx*acz;
    const float nz = abx*acy - aby*acx;

    const float nLen2 = nx*nx + ny*ny + nz*nz;

    // degenerate triangle
    if (nLen2 < 1e-12f) {
        float d0 = segmentSDF(p, a, b);
        float d1 = segmentSDF(p, b, c);
        float d2 = segmentSDF(p, c, a);
        return std::min(d0, std::min(d1, d2));
    }

    // ap
    const float apx = p.x - a.x;
    const float apy = p.y - a.y;
    const float apz = p.z - a.z;

    // distance to plane
    const float distPlane = apx*nx + apy*ny + apz*nz;

    // projection
    const float invN = 1.0f / nLen2;

    const float projx = p.x - nx * distPlane * invN;
    const float projy = p.y - ny * distPlane * invN;
    const float projz = p.z - nz * distPlane * invN;

    // edge tests
    auto edgeTest = [&](const raytracer::Vertice& v0, const raytracer::Vertice& v1, float px, float py, float pz)
    {
        const float ex = v1.x - v0.x;
        const float ey = v1.y - v0.y;
        const float ez = v1.z - v0.z;

        const float vx = px - v0.x;
        const float vy = py - v0.y;
        const float vz = pz - v0.z;

        const float cx = ey*vz - ez*vy;
        const float cy = ez*vx - ex*vz;
        const float cz = ex*vy - ey*vx;

        return (cx*nx + cy*ny + cz*nz) >= 0.f;
    };

    if (edgeTest(a, b, projx, projy, projz) &&
        edgeTest(b, c, projx, projy, projz) &&
        edgeTest(c, a, projx, projy, projz)) {
        return (distPlane * distPlane) * invN;
    }

    // fallback edges
    float d0 = segmentSDF(p, a, b);
    float d1 = segmentSDF(p, b, c);
    float d2 = segmentSDF(p, c, a);

    return std::min(d0, std::min(d1, d2));
}

hot std::pair<float, const raytracer::Face*> raytracer::AObject::computeSDF(const raytracer::Coord& point) const
{
    float sdf = std::numeric_limits<float>::max(), dist = 0.0f;
    const raytracer::Face* sdfFace = nullptr;

    // For each face
    raytracer::Chunk chunk = raytracer::getSpaceChunk(point);
    const auto& faces = this->getObjectDescriptor().faces;
    auto it = faces.find(chunk);
    for (const raytracer::Face& face: unlikely_c(it == faces.end()) ? this->getObjectDescriptor().fallbackFaces : it->second) {
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
