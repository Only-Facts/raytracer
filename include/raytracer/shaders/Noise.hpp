/**************************************************************\
Edition:
##  @date 17/05/2026 by @author Tsukini

File Name:
##  @file Noise.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef NOISE_H
    #define NOISE_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #define _Attribute
    #include "utils/utils.hpp"  // utils::vector::Vector3, nodiscard
    #include "../Define.hpp"    // values
    #include "AShader.hpp"      // raytracer:AShader    
    #include <vector>           // std::vector

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Noise: public raytracer::AShader {
    private:
        /* noise */
        float _strength = NOISE_STRENGTH; // Strength of the noise on the shape color
        float _size = NOISE_SIZE; // Size of the noise (0.05 = big stain, 0.2 = ~normal, 1 = thin, 2 = point)

    public:
        // ---------- Pre-Function -------- //
        void parse(const libconfig::Setting& node) final;
        raytracer::Color scene(const raytracer::Coord& worldPoint, const raytracer::Coord& localPoint, const raytracer::Color& color) final;

        // ------------ Operator ---------- //
        Noise& operator=(const Noise& object) = delete;
        Noise& operator=(Noise&& object) = delete;

        // ---------- Constructor --------- //
        Noise() = default;
        Noise(const Noise& object) = delete;
        Noise(Noise&& object) = delete;

        // ----------- Destructor --------- //
        ~Noise() = default;
};

} // namespace end
#endif /* NOISE_H */
