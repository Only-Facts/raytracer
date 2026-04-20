/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

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

void raytracer::ACamera::reflectRay(std::shared_ptr<raytracer::IRay> ray) const
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Cameras does not support: reflectRay");
}

float raytracer::ACamera::computeSDF(const utils::vector::Vector3<double>& point) const
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Cameras does not support: computeSDF");
}
