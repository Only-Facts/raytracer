/**************************************************************\
Edition:
##  @date 25/04/2026 by @author Tsukini

File Name:
##  @file lib.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/cameras/CameraFOV.hpp"
#include <memory>

extern "C" {
    // Return the name of the lib
    const char* name(void)
    {
        return "CameraFOV";
    }

    // Constructor of the lib
    raytracer::CameraFOV* factory(void)
    {
        return new raytracer::CameraFOV();
    }
}
