/**************************************************************\
Edition:
##  @date 30/04/2026 by @author Tsukini

File Name:
##  @file lib.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/lights/Plane.hpp"
#include <memory>

extern "C" {
    /* Wrapper for Rust Bridge */
    void light_get_position(void* instance, double* x, double* y, double* z) {
        auto* light = static_cast<raytracer::ILight*>(instance);
        auto pos = light->getCFrame().position;
        *x = pos.x; *y = pos.y; *z = pos.z;
    }

    void light_get_color(void* instance, uint8_t* r, uint8_t* g, uint8_t* b, double* intensity) {
        auto* light = static_cast<raytracer::ILight*>(instance);
        auto color = light->getColor();
        *r = (uint8_t)color.x;
        *g = (uint8_t)color.y;
        *b = (uint8_t)color.z;
        *intensity = 1.0;
    }

    void light_set_position(void* instance, double x, double y, double z) {
        auto* light = static_cast<raytracer::ILight*>(instance);
        raytracer::CFrame frame = light->getCFrame();
        frame.position = raytracer::Coord(x, y, z);
        light->setCFrame(frame, true);
    }

    void light_is_global(void* instance, bool* global) {
        auto* light = static_cast<raytracer::ILight*>(instance);
        auto is_global = light->isGlobal();
        *global = is_global;
    }
    /* !Wrapper for Rust Bridge */


    // Return the name of the lib
    const char* name(void)
    {
        return "Plane";
    }

    // Constructor of the lib
    raytracer::Plane* factory(void)
    {
        return new raytracer::Plane();
    }
}
