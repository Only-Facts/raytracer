/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file Raytracer.cpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#include "raytracer/Raytracer.hpp"

void raytracer::Raytracer::gui(void)
{
}

void raytracer::Raytracer::loop(void)
{
}

void raytracer::Raytracer::display(void)
{
}

/*
 0 - Handle input (only in gui mode)
 1 - Reset rays (lights & camera)
 2 - Compute lights rays
    1 - Compute SDF
    2 - Apply SDF (and aproximative gravity curve, only in newton mode)
    3 - Check SDF
    collision ->
        - Reflect
        - Reduce intensity
        - Apply color fusion
        - Add light rays data (hit point, intensity, color) to the object
    too low intensity -> kill
    too far -> kill
 3 - Compute camera rays
    1 - Compute SDF
    2 - Apply SDF (and aproximative gravity curve, only in newton mode)
    3 - Check SDF
    collision ->
        - Reflect (only for mirror material)
        - Apply color (not for mirror material)
        - kill (not for mirror material)
    too far -> kill
*/
void raytracer::Raytracer::render(void)
{
}

void raytracer::Raytracer::loadRender(void)
{
}

void raytracer::Raytracer::saveRender(void)
{
}
