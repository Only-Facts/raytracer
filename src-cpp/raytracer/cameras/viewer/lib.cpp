/**************************************************************\
Edition:
##  @date 26/04/2026 by @author Tsukini

File Name:
##  @file lib.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/cameras/Viewer.hpp"
#include <memory>

extern "C" {
    // Return the name of the lib
    const char* name(void)
    {
        return "Viewer";
    }

    // Constructor of the lib
    raytracer::Viewer* factory(void)
    {
        return new raytracer::Viewer();
    }
}
