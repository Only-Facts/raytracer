/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file Directional.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef DIRECTIONAL_H
    #define DIRECTIONAL_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "ALight.hpp"   // raytracer::ALight

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Directional: public raytracer::ALight {
    public:
        // ---------- Pre-Function -------- //
        void parse(const libconfig::Setting& node);
        void init(void);
        void reset(void);

        // ------------ Operator ---------- //
        Directional& operator=(const Directional& object) = delete;
        Directional& operator=(Directional&& object) = delete;

        // ---------- Constructor --------- //
        Directional() = default;
        Directional(const Directional& object) = delete;
        Directional(Directional&& object) = delete;

        // ----------- Destructor --------- //
        ~Directional() = default;
};

} // namespace end
#endif /* DIRECTIONAL_H */
