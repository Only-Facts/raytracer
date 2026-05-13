/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

File Name:
##  @file lib.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/Struct.hpp"
#include "raytracer/materials/Default.hpp"
#include "raytracer/objects/AObject.hpp"
#include "raytracer/objects/IObject.hpp"
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
    /* !Wrapper for Rust Bridge */


    // Return the name of the lib
    const char* name(void)
    {
        return "Default";
    }

    // Constructor of the lib
    raytracer::Default* factory(void)
    {
        return new raytracer::Default();
    }
}
