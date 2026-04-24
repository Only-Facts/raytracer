/**************************************************************\
Edition:
##  @date 23/04/2026 by @author Tsukini

File Name:
##  @file Cone.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef CONE_H
    #define CONE_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #include "utils/utils.hpp"  // utils::vector::Vector3
    #include "AObject.hpp"      // raytracer::AObject
    #include "../rays/IRay.hpp" // raytracer::IRay
    #include <memory>           // std::shared_ptr

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Cone: public raytracer::AObject {
    public:
        // ---------- Pre-Function -------- //
        void parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node) final;

        // ------------ Operator ---------- //
        Cone& operator=(const Cone& object) = delete;
        Cone& operator=(Cone&& object) = delete;

        // ---------- Constructor --------- //
        Cone() = default;
        Cone(const Cone& object) = delete;
        Cone(Cone&& object) = delete;

        // ----------- Destructor --------- //
        ~Cone() = default;
};

} // namespace end
#endif /* CONE_H */
