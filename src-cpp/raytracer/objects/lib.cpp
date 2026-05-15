/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

File Name:
##  @file lib.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/DynamicLibrary.hpp"
#include "raytracer/materials/IMaterial.hpp"
#include "raytracer/objects/AObject.hpp"
#include <cstddef>
#include <iostream>

// Return the type of the lib
extern "C" {
    /* Wrapper for Rust Bridge */
    struct CSdfResult {
        double distance;
        const void* face_ptr;
    };

    CSdfResult object_compute_sdf(void* instance, double x, double y, double z) {
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

    CHitResult object_compute_hit(void* instance, double px, double py, double pz, const void* face_ptr) {
        raytracer::AObject* obj = static_cast<raytracer::AObject*>(instance);
        const raytracer::Face* face = static_cast<const raytracer::Face*>(face_ptr);
        raytracer::Coord hit = obj->computeHit(raytracer::Coord(px, py, pz), face);
        return CHitResult{ hit.x, hit.y, hit.z };
    }

    struct CColorResult {
        uint8_t r, g, b;
        bool ok;
    };

    CColorResult object_get_point_color(void* instance, double px, double py, double pz) {
        raytracer::AObject* obj = static_cast<raytracer::AObject*>(instance);
 
        if (obj->getObjectDescriptor().material == nullptr) {
            return CColorResult{ 255, 255, 255, true }; // Fallback blanc
        }

        auto color = obj->getObjectDescriptor().material->getColor();
        return CColorResult{ color.x, color.y, color.z, true };
    }

    bool object_is_mirror(void* instance) {
        raytracer::AObject* obj = static_cast<raytracer::AObject*>(instance);

        if (obj->getObjectDescriptor().material == nullptr) {
            return false;
        }

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

    void object_set_color(void* instance, uint8_t r, uint8_t g, uint8_t b) {
        auto* obj = static_cast<raytracer::AObject*>(instance);
        raytracer::ObjectDescriptor desc = obj->getObjectDescriptor();
        if (desc.material) {
            desc.material->setColor(raytracer::Color(r, g, b));
            obj->setObjectDescriptor(desc);
        } else {
            std::cout << "Warning: object has no material" << std::endl;
        }
    }

    void object_set_material(void* instance, void* material_ptr) {
        auto* obj = static_cast<raytracer::AObject*>(instance);
        auto* mat = static_cast<raytracer::IMaterial*>(material_ptr);

        if (obj && mat) {
            obj->getObjectDescriptor().material = mat;
        }
    }
    /* !Wrapper for Rust Bridge */


    std::size_t type(void)
    {
        return OBJECT;
    }
}
