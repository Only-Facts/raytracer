/**************************************************************\
Edition:
##  @date 04/05/2026 by @author Tsukini

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
    #define MAX_LIGHT_RAY 1000000 // Max number of rays by light
    #define CHUNK_SIZE 10 // Size of the chunk used (color, face, sdf)
    #define LIGHT_INTENSITY_LIMIT 1e-5 // Kill the ray under this intensity
    #define SDF_COLLINDING_LIMIT 1e-2 // Detect colliding if the SDF is below or equal to that value
    #define LIGHT_COLOR_LIMIT 4 // Radius of the circle to take color on a shape pixel hit point
    #define MAX_VERTICES 3 // Allow point, segment, triangle
    #define LOAD_FACTOR 0.5f // Load factor used for the unordered map

    /* convertion */
    #define UNIT_METER_COEF 50.0f // Number of local unit that equal to a meter

    /* values */
    #define NO_LIGHT_DEFAULT 2.5f // Allow the incrementation of light even if the value of r, g or b is at 0
    #define RAY_DISTANCE_COEF 2.0f // Coef of the render distance for the distance live of a ray
    #define EXP_K 1.5 // Exponential coef for light diminution with distance (1 = slow, 10 = fast)
    #define NOISE_STRENGTH 0.2f // Strength of the noise on the shape color
    #define NOISE_SIZE 0.1f // Size of the noise (0.05 = big stain, 0.2 = ~normal, 1 = thin, 2 = point)

    /* default */
    #define PLUGINS_PATH "./plugins/"
    #define RENDERED_PATH "./rendered/"
    #define OBJ_PATH "./obj/"
    #define PPM_MAGIC 0x22
    #define DEFAULT_COLOR {0, 0, 0}
    #define DEFAULT_LIGHT false // Enable or disable seeing object without light

#endif /* DEFINE_H */
