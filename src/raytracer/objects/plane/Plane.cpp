/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file Plane.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/objects/Plane.hpp"

void raytracer::Plane::parse(const libconfig::Setting& node)
{
}

void raytracer::Plane::reflectRay(std::shared_ptr<raytracer::IRay> ray) const
{
}

float raytracer::Plane::computeSDF(const utils::vector::Vector3<double>& point) const
{
}
