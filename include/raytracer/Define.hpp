/**************************************************************\
Edition:
##  @date 01/05/2026 by @author Tsukini

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
    #define LIGHT_INTENSITY_LIMIT 1e-2 // Kill the ray under this intensity
    #define SDF_COLLINDING_LIMIT 1e-2 // Detect colliding if the SDF is below or equal to that value
    #define LIGHT_COLOR_LIMIT 4 // Radius of the circle to take color on a shape pixel hit point
    #define MAX_VERTICES 3 // Allow point, segment, triangle

    /* values */
    #define RENDER_DISTANCE 400 // Kill the ray if the SDF is above or equal to that value (can be redefined in the camera)
    #define EXP_K 2 // Exponential coef for light diminution with distance (1 = slow, 10 = fast)
    #define NOISE_STRENGTH 0.2f // Strength of the noise on the shape color
    #define NOISE_SIZE 0.1f // Size of the noise (0.05 = big stain, 0.2 = ~normal, 1 = thin, 2 = point)
    #define LIGHT_RAY 1000000 // Number of rays by light
    #define DEFAULT_LIGHT false // Enable or disable seeing object without light
    #define CHUNK_SIZE 10 // Size of the chunk used

    /* default */
    #define PLUGINS_PATH "./plugins/"
    #define RENDERED_PATH "./rendered/"
    #define OBJ_PATH "./obj/"
    #define PPM_MAGIC 0x22
    #define DEFAULT_COLOR {0, 0, 0}

#endif /* DEFINE_H */
