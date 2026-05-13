/**************************************************************\
Edition:
##  @date 13/05/2026 by @author Tsukini

File Name:
##  @file AMaterial.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef AMATERIAL_H
    #define AMATERIAL_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #define _Attribute
    #include "utils/utils.hpp"  // utils::vector::Vector3, nodiscard
    #include "IMaterial.hpp"    // raytracer:IMaterial
    #include "../Define.hpp"    // values
    #include <limits>           // UINT8_MAX
    #include <vector>           // std::vector

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class AMaterial: public raytracer::IMaterial {
    protected:
        raytracer::Color _color = DEFAULT_COLOR; // Material color

        /* surface */
        float _transparency = 0.0f; // Material transparency
        float _reflection = 0.0f; // Material reflection

        /* noise */
        bool _noise = false; // Enable noise on the surface color
        float _strength = NOISE_STRENGTH; // Strength of the noise on the shape color
        float _size = NOISE_SIZE; // Size of the noise (0.05 = big stain, 0.2 = ~normal, 1 = thin, 2 = point)

    public:
        // ------------ Function ---------- //
        nodiscard raytracer::Color getColor(void) const final {return this->_color;};
        nodiscard float getTransparency(void) const final {return this->_transparency;};
        nodiscard float getReflection(void) const final {return this->_reflection;};
        nodiscard std::pair<float, float> getNoiseSettings(void) const final {return {this->_strength, this->_size};};
        nodiscard bool hasNoise(void) const final {return this->_noise;};

        // ------------ Operator ---------- //
        AMaterial& operator=(const AMaterial& object) = delete;
        AMaterial& operator=(AMaterial&& object) = delete;

        // ---------- Constructor --------- //
        AMaterial() = default;
        AMaterial(const AMaterial& object) = delete;
        AMaterial(AMaterial&& object) = delete;

        // ----------- Destructor --------- //
        ~AMaterial() = default;
};

} // namespace end
#endif /* AMATERIAL_H */
