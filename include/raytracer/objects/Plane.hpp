/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

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
    #define _Vector
    #include "utils/utils.hpp"  // utils::vector::Vector3
    #include "AObject.hpp"      // raytracer::AObject
    #include "../rays/IRay.hpp" // raytracer::IRay
    #include <memory>           // std::shared_ptr

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Plane: public raytracer::AObject {
    public:
        // ---------- Pre-Function -------- //
        void reflectRay(std::shared_ptr<raytracer::IRay> ray) const;
        float computeSDF(const utils::vector::Vector3<double>& point) const;

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
