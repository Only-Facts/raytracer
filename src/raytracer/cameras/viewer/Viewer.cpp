/**************************************************************\
Edition:
##  @date 26/04/2026 by @author Tsukini

File Name:
##  @file Viewer.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/cameras/Viewer.hpp"
#include "raytracer/Struct.hpp"
#include "raytracer/Raytracer.hpp"

void raytracer::Viewer::parse(unused const raytracer::Raytracer& raytracer, unused const libconfig::Setting& node)
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Viewer does not support: parse");
}

void raytracer::Viewer::init(void)
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Viewer does not support: init");
}

void raytracer::Viewer::reset(void)
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Viewer does not support: reset");
}
