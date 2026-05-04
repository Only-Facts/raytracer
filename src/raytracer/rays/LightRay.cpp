/**************************************************************\
Edition:
##  @date 04/05/2026 by @author Tsukini

File Name:
##  @file LightRay.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/rays/LightRay.hpp"

void raytracer::LightRay::reset(void)
{
    this->setImmunity(nullptr);
    this->_alive = true;
    this->_color = DEFAULT_COLOR;
    this->_intensity = 1.0f;
    this->_distance = 0.0f;
    this->_lumen = -1.0f;
}

nodiscard raytracer::LightRay* raytracer::LightRay::clone(void) const
{
    raytracer::LightRay* ray = new LightRay();
    ray->_alive = true; // Security
    ray->_color = this->_color;
    ray->_intensity = this->_intensity;
    ray->_descriptor = this->_descriptor;
    ray->_distance = this->_distance;
    ray->_lumen = this->_lumen;
    return ray;
}

nodiscard float raytracer::LightRay::getLuminescence(void) const
{
    if (this->hasInfinitePower()) {
        return this->_intensity;
    } else {
        raytracer::Type d = this->_distance / UNIT_METER_COEF;
        return this->_intensity * this->_lumen / (4 * M_PI * (d * d));
    }
}
