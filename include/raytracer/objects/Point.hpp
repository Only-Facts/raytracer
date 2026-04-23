/**************************************************************\
Edition:
##  @date 23/04/2026 by @author Tsukini

File Name:
##  @file Point.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef POINT_H
    #define POINT_H

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

class Point: public raytracer::AObject {
    public:
        // ---------- Pre-Function -------- //
        void parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node) final;

        // ------------ Operator ---------- //
        Point& operator=(const Point& object) = delete;
        Point& operator=(Point&& object) = delete;

        // ---------- Constructor --------- //
        Point() = default;
        Point(const Point& object) = delete;
        Point(Point&& object) = delete;

        // ----------- Destructor --------- //
        ~Point() = default;
};

} // namespace end
#endif /* POINT_H */
