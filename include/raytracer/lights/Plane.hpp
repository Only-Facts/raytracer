/**************************************************************\
Edition:
##  @date 13/05/2026 by @author Tsukini

File Name:
##  @file Plane.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef PLANE_H
    #define PLANE_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "ALight.hpp"   // raytracer::ALight

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Plane: public raytracer::ALight {
    public:
        // ---------- Pre-Function -------- //
        void parse(const libconfig::Setting& node) final;
        void init(void) final;
        void reset(void) final;

        // ------------ Operator ---------- //
        Plane& operator=(const Plane& object) = delete;
        Plane& operator=(Plane&& object) = delete;

        // ---------- Constructor --------- //
        Plane() = default;
        Plane(const Plane& object) = delete;
        Plane(Plane&& object) = delete;

        // ----------- Destructor --------- //
        ~Plane() = default;
};

} // namespace end
#endif /* PLANE_H */
