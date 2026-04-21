/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file Cylinder.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef CYLINDER_H
    #define CYLINDER_H

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

class Cylinder: public raytracer::AObject {
    public:
        // ---------- Pre-Function -------- //
        void parse(const libconfig::Setting& node);
        void reflectRay(std::shared_ptr<raytracer::IRay> ray) const;
        float computeSDF(const utils::vector::Vector3<double>& point) const;

        // ------------ Operator ---------- //
        Cylinder& operator=(const Cylinder& object) = delete;
        Cylinder& operator=(Cylinder&& object) = delete;

        // ---------- Constructor --------- //
        Cylinder() = default;
        Cylinder(const Cylinder& object) = delete;
        Cylinder(Cylinder&& object) = delete;

        // ----------- Destructor --------- //
        ~Cylinder() = default;
};

} // namespace end
#endif /* CYLINDER_H */
