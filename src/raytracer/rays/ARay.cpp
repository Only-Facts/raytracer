/**************************************************************\
Edition:
##  @date 01/05/2026 by @author Tsukini

File Name:
##  @file ARay.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/rays/ARay.hpp"

void raytracer::ARay::parse(unused const raytracer::Raytracer& raytracer, unused const libconfig::Setting& node)
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Rays does not support: parse");
}
