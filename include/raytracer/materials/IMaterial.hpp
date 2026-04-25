/**************************************************************\
Edition:
##  @date 24/04/2026 by @author Tsukini

File Name:
##  @file IMaterial.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef IMATERIAL_H
    #define IMATERIAL_H

    //----------------------------------------------------------------//
    /* INCLUDE */
    
    /* type */
    #define _Vector
    #include "utils/utils.hpp"  // utils::vector::Vector3
    #include "../Struct.hpp"    // raytracer::Color
    #include <libconfig.h++>    // libconfig::Setting

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Raytracer;

class IMaterial {
    public:
        // ---------- Pre-Function -------- //
        virtual void parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node) = 0;
        virtual raytracer::Color getColor(void) const = 0;
        virtual float getTransparency(void) const = 0;
        virtual float getLightReflectionCoef(void) const = 0;
        virtual bool isMirror(void) const = 0;

        // ------------ Operator ---------- //
        IMaterial& operator=(const IMaterial& object) = delete;
        IMaterial& operator=(IMaterial&& object) = delete;

        // ---------- Constructor --------- //
        IMaterial() = default;
        IMaterial(const IMaterial& object) = delete;
        IMaterial(IMaterial&& object) = delete;

        // ----------- Destructor --------- //
        virtual ~IMaterial() = default;
};

} // namespace end
#endif /* IMATERIAL_H */
