/**************************************************************\
Edition:
##  @date 25/04/2026 by @author Tsukini

File Name:
##  @file lib.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/lights/Point.hpp"
#include <memory>

extern "C" {
    // Return the name of the lib
    const char* name(void)
    {
        return "Point";
    }

    // Constructor of the lib
    raytracer::Point* factory(void)
    {
        return new raytracer::Point();
    }
}
