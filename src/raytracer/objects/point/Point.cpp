/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file Point.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#include "utils/utils.hpp"
#include "raytracer/objects/Point.hpp"
#include "raytracer/Raytracer.hpp"

void raytracer::Point::parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
}

void raytracer::Point::reflectRay(std::shared_ptr<raytracer::IRay> ray) const
{
}

float raytracer::Point::computeSDF(const utils::vector::Vector3<double>& point) const
{
}
