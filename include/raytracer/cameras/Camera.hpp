/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file Camera.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef CAMERA_H
    #define CAMERA_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "ACamera.hpp"  // raytracer::ACamera

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Camera: public raytracer::ACamera {
    public:
        // ---------- Pre-Function -------- //
        void parse(const libconfig::Setting& node);
        void init(void);
        void reset(void);

        // ------------ Operator ---------- //
        Camera& operator=(const Camera& object) = delete;
        Camera& operator=(Camera&& object) = delete;

        // ---------- Constructor --------- //
        Camera() = default;
        Camera(const Camera& object) = delete;
        Camera(Camera&& object) = delete;

        // ----------- Destructor --------- //
        ~Camera() = default;
};

} // namespace end
#endif /* CAMERA_H */
