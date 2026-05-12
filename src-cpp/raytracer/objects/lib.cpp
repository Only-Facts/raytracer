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
#include <cstddef>

// Return the type of the lib
extern "C" {
    std::size_t type(void)
    {
        return OBJECT;
    }
}
