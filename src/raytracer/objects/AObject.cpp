/**************************************************************\
Edition:
##  @date 24/04/2026 by @author Tsukini

File Name:
##  @file AObject.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#include "utils/utils.hpp"
#include "raytracer/objects/AObject.hpp"
#include "raytracer/rays/IRay.hpp"
#include "raytracer/Struct.hpp"
#include "raytracer/Define.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <algorithm>
#include <exception>
#include <iostream>

utils::vector::Vector3<std::uint8_t> raytracer::AObject::getPointColor(const utils::vector::Vector3<double>& point) const
{
    utils::vector::Vector3<std::uint16_t> pointColor = this->getObjectDescriptor().material->getColor();
    bool found = false;

    // For each light rays
    for (const auto& [position, color, intensity]: this->_lightRays) {
        // Check if the point is near the light ray
        if ((point - position).lengthSquared() > LIGHT_COLOR_LIMIT * LIGHT_COLOR_LIMIT) continue;
        //if ((point - position).length() > LIGHT_COLOR_LIMIT) continue;
        found = true;

        // Fuse the colors
        pointColor = pointColor * color * intensity / 255;
    }

    // No light ray on this hit
    if (!DEFAULT_LIGHT && !found)
        return DEFAULT_COLOR;

    return pointColor;
}

void raytracer::AObject::loadObj(const std::string& path, raytracer::ObjectDescriptor& descriptor)
{
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig config;
    config.triangulate = true;

    // Check error & warning
    if (!reader.ParseFromFile(path, config)) {
        std::string err = reader.Error();
        err.pop_back();
        //utils::exception::CustomException e(utils::exception::Error, utils::exception::Code::Parser, err);
        //std::cout << e.formated() << std::endl;
        throw std::runtime_error(err);
    }
    if (!reader.Warning().empty()) {
        utils::exception::CustomException e(utils::exception::Warning, utils::exception::Code::Parser, reader.Warning());
        std::cout << e.formated() << std::endl;
    }

    // Compute world rotation
    utils::vector::Vector3<double> orientation = descriptor.cframe.orientation;
    double len = orientation.dot(orientation);
    if (len < 1e-12) orientation = {0, 0, 1}; // Fallback orientation
    utils::vector::Vector3<double> forward = orientation.normalize();
    utils::vector::Vector3<double> worldUp = {0, 1, 0};
    if (std::abs(forward.dot(worldUp)) > 0.999) worldUp = {1, 0, 0}; // Edge case, parrallel
    utils::vector::Vector3<double> right = (worldUp.cross(forward)).normalize();
    utils::vector::Vector3<double> up = forward.cross(right);

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
                raytracer::Vertice vertice;

                // Get the vertice
                vertice.x = attrib.vertices[3 * idx.vertex_index + 0];
                vertice.y = attrib.vertices[3 * idx.vertex_index + 1];
                vertice.z = attrib.vertices[3 * idx.vertex_index + 2];

                // Apply rotation & offset
                utils::vector::Vector3<double> rotated = right * vertice.x + up * vertice.y + forward * vertice.z;
                rotated *= descriptor.scale;
                vertice.x = rotated.x + descriptor.cframe.position.x;
                vertice.y = rotated.y + descriptor.cframe.position.y;
                vertice.z = rotated.z + descriptor.cframe.position.z;

                // Store the vertice
                face.push_back(vertice);
            }

            // Store the face
            descriptor.faces.push_back(face);
            indexOffset += verticesCount;
        }
    }
}

void raytracer::AObject::reflectRay(raytracer::IRay* ray) const
{
    raytracer::CFrame cframe = ray->getCFrame();
    utils::vector::Vector3<double> orientation = cframe.orientation.normalize();
    utils::vector::Vector3<double> hit = this->computeHit(cframe.position).normalize();
    if (orientation.dot(hit) > 0) hit = -hit;
    orientation = orientation - hit * (2.0 * orientation.dot(hit));
    cframe.orientation = orientation.normalize();
    ray->setCFrame(cframe);
}

static float segmentSDF(const utils::vector::Vector3<double>& point, const raytracer::Vertice& a, const raytracer::Vertice& b)
{
    utils::vector::Vector3<double> ab = b - a;
    double v = ab.dot(ab);
    if (v == 0.0) return (point - a).length(); // Security
    double t = (point - a).dot(ab) / v;
    t = std::clamp(t, 0.0, 1.0);
    utils::vector::Vector3<double> closest = a + t * ab;
    return (point - closest).length();
}

/*
static float triangleSDF(const utils::vector::Vector3<double>& point, const raytracer::Vertice& a, const raytracer::Vertice& b, const raytracer::Vertice& c)
{
    utils::vector::Vector3<double> ba = b - a, pa = point - a;
    utils::vector::Vector3<double> cb = c - b, pb = point - b;
    utils::vector::Vector3<double> ac = a - c, pc = point - c;
    utils::vector::Vector3<double> nor = ba.cross(ac);

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

static float triangleSDF(const utils::vector::Vector3<double>& point, const raytracer::Vertice& a, const raytracer::Vertice& b, const raytracer::Vertice& c)
{
    utils::vector::Vector3<double> ab = b - a;
    utils::vector::Vector3<double> ac = c - a;
    utils::vector::Vector3<double> ap = point - a;

    // normale du triangle
    utils::vector::Vector3<double> n = ab.cross(ac);
    double nLen2 = n.dot(n);

    // Invalid triangle
    if (nLen2 == 0.0) {
        return std::min({
            segmentSDF(point, a, b),
            segmentSDF(point, b, c),
            segmentSDF(point, c, a)
        });
    }

    // Project the point on the triangle
    double distPlane = ap.dot(n) / std::sqrt(nLen2);
    utils::vector::Vector3<double> proj = point - n * (distPlane / std::sqrt(nLen2));

    // Check if the point is inside
    utils::vector::Vector3<double> bp = proj - b;
    utils::vector::Vector3<double> cp = proj - c;

    utils::vector::Vector3<double> bc = c - b;
    utils::vector::Vector3<double> ca = a - c;

    // Edge case, same orientation
    if ((ab.cross(proj - a)).dot(n) >= 0
        && (bc.cross(bp)).dot(n) >= 0
        && (ca.cross(cp)).dot(n) >= 0)
        return std::abs(distPlane);

    // Edge case, on side
    return std::min({
        segmentSDF(point, a, b),
        segmentSDF(point, b, c),
        segmentSDF(point, c, a)
    });
}

float raytracer::AObject::computeSDF(const utils::vector::Vector3<double>& point) const
{
    float sdf = std::numeric_limits<float>::max(), dist = 0.0f;

    // For each face
    for (const raytracer::Face& face: this->getObjectDescriptor().faces) {
        dist = 0.0f;

        // Dispatch the computing
        switch (face.size()) {
            case 1: dist = (point - face[0]).length();                      break;
            case 2: dist = segmentSDF(point, face[0], face[1]);             break;
            case 3: dist = triangleSDF(point, face[0], face[1], face[2]);   break;
            default:
                throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, "Invalid number of vertices for a face on the object to render");
        }

        sdf = std::min(sdf, dist);
    }

    return sdf;
}

utils::vector::Vector3<double> raytracer::AObject::computeHit(const utils::vector::Vector3<double>& point) const
{
    return {0.0, 0.0, 0.0};
}
