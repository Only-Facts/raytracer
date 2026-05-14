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
    void light_init(void* instance) {
        static_cast<raytracer::ILight*>(instance)->init();
    }
    void light_parse(void* instance, const char* config_json) {
        // TODO: json to settings
    }
    std::size_t type(void)
    {
        return LIGHT;
    }
}
