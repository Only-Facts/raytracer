/**************************************************************\
Edition:
##  @date 03/05/2026 by @author Tsukini

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
    #define _Attribute
    #include "utils/utils.hpp"          // utils::vector::Vector3, nodiscard
    #include "ARay.hpp"                 // raytracer::ARay
    #include "../Struct.hpp"            // raytracer::Color
    #include <cstdint>                  // std::uint8_t

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Ray: public raytracer::ARay {
    private:
        raytracer::Color _color; // RGB color that what hit, add a percentage of the color from the surface hitted each time
        float _coef = 1.0f; // Coef reduced at each color added

    public:
        // ---------- Pre-Function -------- //
        void reset(void);

        // ------------ Function ---------- //
        void setCoef(float coef) {this->_coef = coef;};
        nodiscard float getCoef(void) const {return this->_coef;};
        void setColor(raytracer::Color color) {this->_color = color;};
        nodiscard raytracer::Color getColor(void) const {return this->_color;};

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
