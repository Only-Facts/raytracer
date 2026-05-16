/**************************************************************\
Edition:
##  @date 16/05/2026 by @author Tsukini

File Name:
##  @file Define.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef DEFINE_H
    #define DEFINE_H

    //----------------------------------------------------------------//
    /* DEFINE */

    /* limits */
    #define RENDER_DISTANCE 400 // Kill the ray if the SDF is above or equal to that value (can be redefined in the camera)
    #define MAX_LIGHT_RAY 10000//00 // Max number of rays by light
    #define LIGHT_INTENSITY_LIMIT 1e-5 // Kill the ray under this intensity
    #define CAMERA_COEF_LIMIT 1e-5 // Kill the ray under this coef
    #define SDF_COLLINDING_LIMIT 1e-2 // Detect colliding if the SDF is below or equal to that value
    #define LIGHT_COLOR_LIMIT 4.0f // Radius of the circle to take color on a shape pixel hit point
    #define MAX_VERTICES 3 // Allow point, segment, triangle
    #define LOAD_FACTOR 0.5f // Load factor used for the unordered map
    #define SPACE_CHUNK_SIZE 100.0f // Size of the chunk used (color, face, sdf)
    #define COLOR_CHUNK_SIZE (LIGHT_COLOR_LIMIT * 2.5) // Size of the chunk used (color, face, sdf)
    #define RAY_MAX_USELESS_LOOP(renderDistance) (renderDistance / SPACE_CHUNK_SIZE) // Number of useless loop allowed before getting killed
    #define RAY_MAX_DEPTH 25 // Depth maximal of cloning for the rays
    #define MAX_STEP 3 // Number of adv max step

    /* convertion */
    #define UNIT_METER_COEF 50.0f // Number of local unit that equal to a meter

    /* values */
    #define ADV_SIZE 75 // Size of the advencement display
    #define NO_LIGHT_DEFAULT 2.5f // Allow the incrementation of light even if the value of r, g or b is at 0
    #define RAY_DISTANCE_COEF 2.0f // Coef of the render distance for the distance live of a ray
    #define EXP_K 1.5 // Exponential coef for light diminution with distance (1 = slow, 10 = fast)
    #define NOISE_STRENGTH 0.2f // Strength of the noise on the shape color
    #define NOISE_SIZE 0.1f // Size of the noise (0.05 = big stain, 0.2 = ~normal, 1 = thin, 2 = point)
    #define MOVE_SPEED 10.0f
    #define ORIENTATION_SPEED 1.0f
    #define SOLAR_MASS 1.988e30 // Used as unit for singularity (default: kg)
    #define PHOTON_MASS 1e-10 // Mass of the rays used for newton
    #define G_NEWTON 6.674e-11 // Newtonian constant of gravitation

    /* default */
    #define PLUGINS_PATH "./plugins/"
    #define RENDERED_PATH "./rendered/"
    #define OBJ_PATH "./obj/"
    #define PPM_MAGIC 0x22
    #define DEFAULT_COLOR {0, 0, 0}
    #define DEFAULT_LIGHT false // Enable or disable seeing object without light

#endif /* DEFINE_H */
