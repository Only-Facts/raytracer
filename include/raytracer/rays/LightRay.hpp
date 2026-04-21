/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

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
    #include "utils/utils.hpp"  // utils::vector::Vector3
    #include "IRay.hpp"         // raytracer::IRay
    #include <cstdint>          // std::uint8_t

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class LightRay: public raytracer::IRay {
    private:
        utils::vector::Vector3<std::uint8_t> _color; // Color of the light
        float _intensity; // Intensity of the light, decrease at each surface hitted

    public:
        // ---------- Pre-Function -------- //
        void reset(void);

        // ------------ Function ---------- //
        void setColor(utils::vector::Vector3<std::uint8_t> color) {this->_color = color;};
        utils::vector::Vector3<std::uint8_t> getColor(void) const {return this->_color;};
        void setIntensity(float intensity) {this->_intensity = intensity;};
        float getIntensity(void) const {return this->_intensity;};

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
