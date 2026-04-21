/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file Cylinder.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#include "utils/utils.hpp"
#include "raytracer/objects/Cylinder.hpp"
#include "raytracer/Raytracer.hpp"

void raytracer::Cylinder::parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node)
{
}

void raytracer::Cylinder::reflectRay(std::shared_ptr<raytracer::IRay> ray) const
{
}

float raytracer::Cylinder::computeSDF(const utils::vector::Vector3<double>& point) const
{
}
