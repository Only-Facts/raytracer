/**************************************************************\
Edition:
##  @date 25/04/2026 by @author Tsukini

File Name:
##  @file Viewer.hpp

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

class Viewer: public raytracer::ACamera {
    public:
        // ---------- Pre-Function -------- //
        void parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node) final;
        void init(void) final;
        void reset(void) final;

        // ------------ Operator ---------- //
        Viewer& operator=(const Viewer& object) = delete;
        Viewer& operator=(Viewer&& object) = delete;

        // ---------- Constructor --------- //
        Viewer() = default;
        Viewer(const Viewer& object) = delete;
        Viewer(Viewer&& object) = delete;

        // ----------- Destructor --------- //
        ~Viewer() = default;
};

} // namespace end
#endif /* CAMERA_H */
