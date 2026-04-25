/**************************************************************\
Edition:
##  @date 25/04/2026 by @author Tsukini

File Name:
##  @file Point.hpp

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

class Point: public raytracer::ALight {
    public:
        // ---------- Pre-Function -------- //
        void parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node) final;
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
#endif /* AMBIENT_H */
