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
#include "raytracer/cameras/ICamera.hpp"
#include <cstddef>

// Return the type of the lib
extern "C" {
    const raytracer::Color* camera_get_pixels(void* instance) {
        raytracer::ICamera* cam = static_cast<raytracer::ICamera*>(instance);
        return cam->getScreen().data();
    }
    void camera_render(void* instance) {
        static_cast<raytracer::ICamera*>(instance)->updateScreen();
    }
    std::size_t type(void)
    {
        return CAMERA;
    }
}
