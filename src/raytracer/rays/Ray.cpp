/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file Ray.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/rays/Ray.hpp"
#include <limits>

void raytracer::Ray::reset(void)
{
    this->_color = DEFAULT_COLOR;
}
