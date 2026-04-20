/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

File Name:
##  @file Ray.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef RAY_H
    #define RAY_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #include "utils/utils.hpp"          // utils::vector::Vector3
    #include "IRay.hpp"                 // raytracer::IRay
    #include <cstdint>                  // std::uint8_t

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Ray: public raytracer::IRay {
    private:
        utils::vector::Vector3<std::uint8_t> _color; // RGB color that what hit, add a percentage of the color from the surface hitted each time

    public:
        // ---------- Pre-Function -------- //
        void reset(void);

        // ------------ Function ---------- //
        void setColor(utils::vector::Vector3<std::uint8_t> color) {this->_color = color;};
        utils::vector::Vector3<std::uint8_t> getColor(void) const {return this->_color;};

        // ------------ Operator ---------- //
        Ray& operator=(const Ray& object) = delete;
        Ray& operator=(Ray&& object) = delete;

        // ---------- Constructor --------- //
        Ray() = default;
        Ray(const Ray& object) = delete;
        Ray(Ray&& object) = delete;

        // ----------- Destructor --------- //
        ~Ray() = default;
};

} // namespace end
#endif /* RAY_H */
