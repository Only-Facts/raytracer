/**************************************************************\
Edition:
##  @date 22/04/2026 by @author Tsukini

File Name:
##  @file LightRay.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/rays/LightRay.hpp"

void raytracer::LightRay::reset(void)
{
    this->_alive = true;
    this->_color = DEFAULT_COLOR;
    this->_intensity = 1.0f;
}
