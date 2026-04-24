/**************************************************************\
Edition:
##  @date 23/04/2026 by @author Tsukini

File Name:
##  @file Ambient.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef AMBIENT_H
    #define AMBIENT_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "ALight.hpp"   // raytracer::ALight

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Ambient: public raytracer::ALight {
    public:
        // ---------- Pre-Function -------- //
        void parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node) final;
        void init(void) final;
        void reset(void) final;

        // ------------ Operator ---------- //
        Ambient& operator=(const Ambient& object) = delete;
        Ambient& operator=(Ambient&& object) = delete;

        // ---------- Constructor --------- //
        Ambient() = default;
        Ambient(const Ambient& object) = delete;
        Ambient(Ambient&& object) = delete;

        // ----------- Destructor --------- //
        ~Ambient() = default;
};

} // namespace end
#endif /* AMBIENT_H */
