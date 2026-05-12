/**************************************************************\
Edition:
##  @date 02/05/2026 by @author Tsukini

File Name:
##  @file ALight.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/lights/ALight.hpp"

raytracer::ALight::~ALight()
{
    for (raytracer::LightRay* ray: this->_rays)
        delete ray;
}
