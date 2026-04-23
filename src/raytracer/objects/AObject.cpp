/**************************************************************\
Edition:
##  @date 23/04/2026 by @author Tsukini

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
#include <iostream>

utils::vector::Vector3<std::uint8_t> raytracer::AObject::getPointColor(const utils::vector::Vector3<double>& point) const
{
    utils::vector::Vector3<std::uint8_t> pointColor = this->getObjectDescriptor().material->getColor();
    bool found = false;

    // For each light rays
    for (const auto& [position, color, intensity]: this->_lightRays) {
        // Check if the point is near the light ray
        if ((point - position).length() > LIGHT_COLOR_LIMIT) continue;
        found = true;

        // Fuse the colors
        pointColor = std::clamp(
            static_cast<utils::vector::Vector3<float>>(pointColor).normalize() *
            static_cast<utils::vector::Vector3<float>>(color).normalize() *
            intensity,
            {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}
        ) * 255;
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
    if (!reader.ParseFromFile(path, config))
        throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, reader.Error());
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
        for (std::size_t f = 0, indexOffset = 0; f < shape.mesh.num_face_vertices.size(); ++f, indexOffset += 3) {
            // Check the number of vertices
            std::size_t verticesCount = shape.mesh.num_face_vertices[f];
            if (verticesCount < MAX_VERTICES)
                throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::Parser, std::string("Invalid vertices number, sould be inferior or equal to '") + std::to_string(MAX_VERTICES) + "', but got: " + std::to_string(f));

            // Setup the vertex
            raytracer::Face face;
            for (std::size_t v = 0; v < verticesCount; ++v) {
                const tinyobj::index_t& idx = shape.mesh.indices[indexOffset + v];
                raytracer::Vertice vertice;
                vertice.x = attrib.vertices[3 * idx.vertex_index + 0];
                vertice.y = attrib.vertices[3 * idx.vertex_index + 1];
                vertice.z = attrib.vertices[3 * idx.vertex_index + 2];
                face.push_back(vertice);
            }

            // Store the vertices
            descriptor.faces.push_back(face);
        }
    }
}

void raytracer::AObject::reflectRay(std::shared_ptr<raytracer::IRay> ray) const
{
    raytracer::CFrame cframe = ray->getCFrame();
    utils::vector::Vector3<double> orientation = cframe.orientation.normalize();
    utils::vector::Vector3<double> hit = this->computeHit(cframe.position).normalize();
    if (orientation.dot(hit) > 0) hit = -hit;
    orientation = orientation - hit * (2.0 * orientation.dot(hit));
    cframe.orientation = orientation.normalize();
    ray->setCFrame(cframe);
}

float raytracer::AObject::computeSDF(const utils::vector::Vector3<double>& point) const
{
    return 0.0;
}

utils::vector::Vector3<double> raytracer::AObject::computeHit(const utils::vector::Vector3<double>& point) const
{
    return {0.0, 0.0, 0.0};
}
