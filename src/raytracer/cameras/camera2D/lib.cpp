/**************************************************************\
Edition:
##  @date 25/04/2026 by @author Tsukini

File Name:
##  @file lib.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/cameras/Camera2D.hpp"
#include <memory>

extern "C" {
    // Return the name of the lib
    const char* name(void)
    {
        return "Camera2D";
    }

    // Constructor of the lib
    raytracer::Camera2D* factory(void)
    {
        return new raytracer::Camera2D();
    }
}
