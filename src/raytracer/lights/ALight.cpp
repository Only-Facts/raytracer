/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file ALight.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/lights/ALight.hpp"

void raytracer::ALight::reflectRay(std::shared_ptr<raytracer::IRay> ray) const
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Lights does not support: reflectRay");
}

float raytracer::ALight::computeSDF(const utils::vector::Vector3<double>& point) const
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Lights does not support: computeSDF");
}
