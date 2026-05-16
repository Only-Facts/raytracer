/**************************************************************\
Edition:
##  @date 16/05/2026 by @author Tsukini

File Name:
##  @file Ray.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/rays/Ray.hpp"
#include "raytracer/special/Utils.hpp"

void raytracer::Ray::reset(void)
{
    this->setImmunity(nullptr);
    this->_alive = true;
    this->_color = DEFAULT_COLOR;
    this->_distance = 0.0f;
    this->_coef = 1.0f;
    this->_descriptor.gravity.acceleration = {0.0, 0.0, 0.0};
    this->_descriptor.gravity.velocity = {0.0, 0.0, 0.0};
}

nodiscard void* raytracer::Ray::clone(void)
{
    raytracer::Ray* ray = new Ray();
    ray->_alive = true; // Security
    ray->_color = this->_color;
    ray->_coef = this->_coef;
    ray->_descriptor = this->_descriptor;
    ray->_distance = this->_distance;
    ray->_ray = this->_ray ? this->_ray : this;
    return ray;
}

raytracer::Ray::~Ray()
{
    // Apply values to the parent
    if (!this->_ray) return;
    this->_ray->setColor(raytracer::moyColor(this->_ray->getColor(), this->getColor()));
}
