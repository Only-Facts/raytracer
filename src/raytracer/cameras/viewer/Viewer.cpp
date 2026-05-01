/**************************************************************\
Edition:
##  @date 28/04/2026 by @author Tsukini

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
    std::size_t size = static_cast<std::size_t>(this->getResolution().x) * this->getResolution().y;

    // Clear old data
    this->_screen.clear();
    for (std::size_t i = 0; i < this->_rays.size(); ++i)
        delete this->_rays[i];
    this->_rays.clear();

    // Resize screen size
    this->_screen.reserve(size);
    this->_rays.reserve(size);
    this->_screen.resize(size, DEFAULT_COLOR);
    this->_rays.resize(size, nullptr);
    for (std::size_t i = 0; i < size; ++i)
        this->_rays[i] = new raytracer::Ray();
}

void raytracer::Viewer::reset(void)
{
    throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NotSupportedFunction, "Viewer does not support: reset");
}
