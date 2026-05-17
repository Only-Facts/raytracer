/**************************************************************\
Edition:
##  @date 17/05/2026 by @author Tsukini

File Name:
##  @file ACamera.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/cameras/ACamera.hpp"
#include <cstdint>

void raytracer::ACamera::updateScreen(bool deadColor)
{
    // For each rays setup the screen corresponding pixel
    for (std::size_t i = 0; i < this->_rays.size(); ++i) {
        if (this->_rays[i]->isAlive()) {
            if (deadColor) this->_screen[i] = DEAD_COLOR;
            continue;
        }
        this->_screen[i] = this->_rays[i]->getColor();
    }
}

void raytracer::ACamera::kill(void)
{
    for (std::size_t i = 0; i < this->_rays.size(); ++i)
        this->_rays[i]->kill();
}

raytracer::ACamera::~ACamera()
{
    for (raytracer::Ray* ray: this->_rays)
        delete ray;
}
