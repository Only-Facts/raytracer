/**************************************************************\
Edition:
##  @date 22/04/2026 by @author Tsukini

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

void raytracer::ACamera::reflectRay(unused std::shared_ptr<raytracer::IRay> ray) const
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Cameras does not support: reflectRay");
}

float raytracer::ACamera::computeSDF(unused const utils::vector::Vector3<double>& point) const
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Cameras does not support: computeSDF");
}

utils::vector::Vector3<double> raytracer::ACamera::computeHit(unused const utils::vector::Vector3<double>& point) const
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Cameras does not support: computeHit");
}

void raytracer::ACamera::updateScreen(void)
{
    // For each rays setup the screen corresponding pixel
    for (std::size_t i = 0; i < this->_rays.size(); ++i)
        this->_screen[i] = this->_rays[i]->getColor();
}
