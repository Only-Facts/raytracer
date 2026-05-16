/**************************************************************\
Edition:
##  @date 13/05/2026 by @author Tsukini

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
    #include "ALight.hpp"   // raytracer::ALight

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Point: public raytracer::ALight {
    public:
        // ---------- Pre-Function -------- //
        void parse(const libconfig::Setting& node) final;
        void init(void) final;
        void reset(void) final;

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
