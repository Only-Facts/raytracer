/**************************************************************\
Edition:
##  @date 13/05/2026 by @author Tsukini

File Name:
##  @file Object.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef OBJECT_H
    #define OBJECT_H

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

class Object: public raytracer::AObject {
    public:
        // ---------- Pre-Function -------- //
        void parse(const libconfig::Setting& node) final;

        // ------------ Operator ---------- //
        Object& operator=(const Object& object) = delete;
        Object& operator=(Object&& object) = delete;

        // ---------- Constructor --------- //
        Object() = default;
        Object(const Object& object) = delete;
        Object(Object&& object) = delete;

        // ----------- Destructor --------- //
        ~Object() = default;
};

} // namespace end
#endif /* OBJECT_H */
