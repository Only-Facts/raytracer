/**************************************************************\
Edition:
##  @date 13/05/2026 by @author Tsukini

File Name:
##  @file Spot.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef SPOT_H
    #define SPOT_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "ALight.hpp"   // raytracer::ALight

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Spot: public raytracer::ALight {
    private:
        float _fieldOfLight = 70.0f;

    public:
        // ---------- Pre-Function -------- //
        void parse(const libconfig::Setting& node) final;
        void init(void) final;
        void reset(void) final;

        // ------------ Operator ---------- //
        Spot& operator=(const Spot& object) = delete;
        Spot& operator=(Spot&& object) = delete;

        // ---------- Constructor --------- //
        Spot() = default;
        Spot(const Spot& object) = delete;
        Spot(Spot&& object) = delete;

        // ----------- Destructor --------- //
        ~Spot() = default;
};

} // namespace end
#endif /* SPOT_H */
