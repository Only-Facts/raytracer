/**************************************************************\
Edition:
##  @date 25/04/2026 by @author Tsukini

File Name:
##  @file Camera2D.hpp

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

class Camera2D: public raytracer::ACamera {
    public:
        // ---------- Pre-Function -------- //
        void parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node) final;
        void init(void) final;
        void reset(void) final;

        // ------------ Operator ---------- //
        Camera2D& operator=(const Camera2D& object) = delete;
        Camera2D& operator=(Camera2D&& object) = delete;

        // ---------- Constructor --------- //
        Camera2D() = default;
        Camera2D(const Camera2D& object) = delete;
        Camera2D(Camera2D&& object) = delete;

        // ----------- Destructor --------- //
        ~Camera2D() = default;
};

} // namespace end
#endif /* CAMERA_H */
