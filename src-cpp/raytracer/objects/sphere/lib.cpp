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
#include "raytracer/objects/Sphere.hpp"
#include <iostream>
#include <memory>

extern "C" {
    /* Wrapper for Rust Bridge */
    struct CSdfResult {
        double distance;
        const void* face_ptr;
    };

    void object_set_radius(void* instance, double r) {
        auto* sphere = dynamic_cast<raytracer::Sphere*>(static_cast<raytracer::AObject*>(instance));
        sphere->setRadius(r);
        std::cout << "radius: " << r << std::endl;
    }

    CSdfResult sphere_compute_sdf(void* instance, double x, double y, double z) {
        auto* sphere = static_cast<raytracer::Sphere*>(instance);
        auto result = sphere->computeSDF(raytracer::Coord(x, y, z));

        //std::cout << sphere->getCFrame().position << std::endl;
        //std::cout << result.first << std::endl << x << " " << y << " " << z << std::endl;

        return CSdfResult { result.first, static_cast<const void*>(result.second) };
    }

    struct CHitResult {
        double nx, ny, nz;
    };

    CHitResult sphere_compute_hit(void* instance, double px, double py, double pz, const void* face_ptr) {
        raytracer::Sphere* obj = static_cast<raytracer::Sphere*>(instance);
        const raytracer::Face* face = static_cast<const raytracer::Face*>(face_ptr);
        raytracer::Coord hit = obj->computeHit(raytracer::Coord(px, py, pz), face);
        return CHitResult{ hit.x, hit.y, hit.z };
    }
    /* !Wrapper for Rust Bridge */

    // Return the name of the lib
    const char* name(void)
    {
        return "Sphere";
    }

    // Constructor of the lib
    raytracer::Sphere* factory(void)
    {
        return new raytracer::Sphere();
    }
}
