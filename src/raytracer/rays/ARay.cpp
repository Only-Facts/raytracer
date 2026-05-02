/**************************************************************\
Edition:
##  @date 02/05/2026 by @author Tsukini

File Name:
##  @file ARay.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/rays/ARay.hpp"
#include <unordered_set>

void raytracer::ARay::parse(unused const raytracer::Raytracer& raytracer, unused const libconfig::Setting& node)
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Rays does not support: parse");
}

void raytracer::ARay::computeObjects(raytracer::Type renderDistance, const std::vector<raytracer::IObject*>& objects, const std::unordered_map<raytracer::Chunk, std::vector<raytracer::IObject*>, raytracer::ChunkHash>& objectsChunks)
{
    raytracer::Type renderDistanceSquared = renderDistance * renderDistance;
    raytracer::CFrame cframe = this->getCFrame();
    raytracer::Coord positionOrigin = cframe.position;
    raytracer::Direction direction = cframe.orientation * CHUNK_SIZE;
    raytracer::Chunk chunk;
    std::unordered_set<IObject*> seen;
    seen.reserve(objects.size());

    // Reset
    this->_objects.clear();
    this->_objects.reserve(objects.size());
    if (objects.size() == 0) return;

    // Get every object other than *.obj
    for (raytracer::IObject* object: objects) {
        if (object->getObjectDescriptor().faces.size() == 0) {
            this->_objects.push_back(object);
            seen.insert(object);
        }
    }
    if (seen.size() == objects.size()) return;

    // Get the ray chunks (While not out of bounds)
    for (; (cframe.position - positionOrigin).lengthSquared() < renderDistanceSquared; cframe.position += direction) {
        chunk = raytracer::getChunk(cframe.position);
        // Get the shape that will intersect with the chunk
        auto it = objectsChunks.find(chunk);
        if (it != objectsChunks.end()) {
            for (raytracer::IObject* object: it->second) {
                if (!seen.insert(object).second) continue;
                this->_objects.push_back(object);
            }
        }
        if (seen.size() == objects.size()) return;
    }
}
