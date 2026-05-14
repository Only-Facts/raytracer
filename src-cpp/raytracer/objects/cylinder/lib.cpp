/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

File Name:
##  @file lib.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/objects/Cylinder.hpp"
#include <memory>

extern "C" {
    /* Wrapper for Rust Bridge */
    struct CSdfResult {
        double distance;
        const void* face_ptr;
    };

    CSdfResult object_compute_sdf(void* instance, float x, float y, float z) {
        raytracer::AObject* obj = static_cast<raytracer::AObject*>(instance);
        auto result = obj->computeSDF(raytracer::Coord(x, y, z));

        return CSdfResult { result.first, static_cast<const void*>(result.second) };
    }

    void destroy_instance(void* instance) {
        delete static_cast<raytracer::IObject*>(instance);
    }

    struct CHitResult {
        double nx, ny, nz;
    };

    struct CColorResult {
        uint8_t r, g, b;
        bool ok;
    };

    CHitResult object_compute_hit(void* instance, double px, double py, double pz, const void* face_ptr) {
        raytracer::AObject* obj = static_cast<raytracer::AObject*>(instance);
        const raytracer::Face* face = static_cast<const raytracer::Face*>(face_ptr);
        raytracer::Coord hit = obj->computeHit(raytracer::Coord(px, py, pz), face);
        return CHitResult{ hit.x, hit.y, hit.z };
    }

    CColorResult object_get_point_color(void* instance, double px, double py, double pz) {
        raytracer::AObject* obj = static_cast<raytracer::AObject*>(instance);
        auto result = obj->getPointColor(raytracer::Coord(px, py, pz));
        return CColorResult{ result.first.x, result.first.y, result.first.z, result.second };
    }

    bool object_is_mirror(void* instance) {
        raytracer::AObject* obj = static_cast<raytracer::AObject*>(instance);
        return obj->getObjectDescriptor().material->isMirror();
    }

    void object_set_position(void* instance, double x, double y, double z) {
        raytracer::AObject* obj = static_cast<raytracer::AObject*>(instance);
        raytracer::CFrame frame = obj->getCFrame();
        frame.position = raytracer::Coord(x, y, z);
        obj->setCFrame(frame, true);
    }

    void object_set_orientation(void* instance, double x, double y, double z) {
        raytracer::AObject* obj = static_cast<raytracer::AObject*>(instance);
        raytracer::CFrame frame = obj->getCFrame();
        frame.orientation = raytracer::Coord(x, y, z);
        obj->setCFrame(frame, true);
    }
    /* !Wrapper for Rust Bridge */


    // Return the name of the lib
    const char* name(void)
    {
        return "Cylinder";
    }

    // Constructor of the lib
    raytracer::Cylinder* factory(void)
    {
        return new raytracer::Cylinder();
    }
}
