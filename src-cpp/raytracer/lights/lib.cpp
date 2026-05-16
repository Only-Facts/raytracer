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
#include "raytracer/lights/ILight.hpp"
#include <cstddef>

// Return the type of the lib
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
        *intensity = light->getIntensity();
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

    void destroy_instance(void* instance) {
        delete static_cast<raytracer::ILight*>(instance);
    }

    void light_set_color(void* instance, uint8_t r, uint8_t g, uint8_t b) {
        auto* light = static_cast<raytracer::ILight*>(instance);
        light->setColor(raytracer::Color(r, g, b));
    }

    void light_set_intensity(void* instance, double intensity) {
        auto* light = static_cast<raytracer::ILight*>(instance);
        light->setIntensity(static_cast<float>(intensity));
    }
    /* !Wrapper for Rust Bridge */

    void light_init(void* instance) {
        static_cast<raytracer::ILight*>(instance)->init();
    }

    std::size_t type(void)
    {
        return LIGHT;
    }
}
