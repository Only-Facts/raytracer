/**************************************************************\
Edition:
##  @date 22/04/2026 by @author Tsukini

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

    /* values */
    #define LIGHT_INTENSITY_LIMIT 1e8 // Kill the ray under this intensity
    #define SDF_COLLINDING_LIMIT 1e8 // Detect colliding if the SDF is below or equal to that value
    #define RENDER_DISTANCE 100 // Kill the ray if the SDF is above or equal to that value

    /* default */
    #define PLUGINS_PATH "./plugins/"
    #define RENDERED_PATH "./rendered/"
    #define PPM_MAGIC 0x22
    #define DEFAULT_COLOR {0, 0, 0}

#endif /* DEFINE_H */
