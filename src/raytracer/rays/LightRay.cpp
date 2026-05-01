/**************************************************************\
Edition:
##  @date 01/05/2026 by @author Tsukini

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

nodiscard raytracer::LightRay* raytracer::LightRay::clone(void) const
{
    raytracer::LightRay* ray = new LightRay();
    ray->_alive = true; // Security
    ray->_color = this->_color;
    ray->_intensity = this->_intensity;
    ray->_descriptor = this->_descriptor;
    return ray;
}
