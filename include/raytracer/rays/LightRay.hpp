/**************************************************************\
Edition:
##  @date 24/04/2026 by @author Tsukini

File Name:
##  @file LightRay.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef LIGHTRAY_H
    #define LIGHTRAY_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #define _Attribute
    #include "utils/utils.hpp"  // utils::vector::Vector3, nodiscard
    #include "ARay.hpp"         // raytracer::ARay
    #include "../Struct.hpp"    // raytracer::Color
    #include <cstdint>          // std::uint8_t

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class LightRay: public raytracer::ARay {
    private:
        raytracer::Color _color; // Color of the light
        float _intensity; // Intensity of the light, decrease at each surface hitted

    public:
        // ---------- Pre-Function -------- //
        void reset(void) final;

        // ------------ Function ---------- //
        void setColor(raytracer::Color color) {this->_color = color;};
        nodiscard raytracer::Color getColor(void) const {return this->_color;};
        void setIntensity(float intensity) {this->_intensity = intensity;};
        nodiscard float getIntensity(void) const {return this->_intensity;};

        // ------------ Operator ---------- //
        LightRay& operator=(const LightRay& object) = delete;
        LightRay& operator=(LightRay&& object) = delete;

        // ---------- Constructor --------- //
        LightRay() = default;
        LightRay(const LightRay& object) = delete;
        LightRay(LightRay&& object) = delete;

        // ----------- Destructor --------- //
        ~LightRay() = default;
};

} // namespace end
#endif /* LIGHTRAY_H */
