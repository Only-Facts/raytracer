/**************************************************************\
Edition:
##  @date 17/05/2026 by @author Tsukini

File Name:
##  @file AShader.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef ASHADER_H
    #define ASHADER_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #define _Attribute
    #include "utils/utils.hpp"  // utils::vector::Vector3, nodiscard
    #include "IShader.hpp"      // raytracer:IShader
    #include "../Define.hpp"    // values
    #include <limits>           // UINT8_MAX
    #include <vector>           // std::vector

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class AShader: public raytracer::IShader {
    public:
        // ------------ Function ---------- //
        void parse(unused const libconfig::Setting& node) override {/* nothing */};
        hot nodiscard raytracer::Color screen(unused const raytracer::Coord2D& point, const raytracer::Color& pixel) override {return pixel;};
        hot nodiscard raytracer::Color scene(unused const raytracer::Coord& worldPoint, unused const raytracer::Coord& localPoint, const raytracer::Color& color) override {return color;};
        hot nodiscard bool light(unused raytracer::LightRay* ray, unused raytracer::IObject* object, unused float sdf) override {return false;};
        void material(unused raytracer::IMaterial* material) override {/* nothing */};

        // ------------ Operator ---------- //
        AShader& operator=(const AShader& object) = delete;
        AShader& operator=(AShader&& object) = delete;

        // ---------- Constructor --------- //
        AShader() = default;
        AShader(const AShader& object) = delete;
        AShader(AShader&& object) = delete;

        // ----------- Destructor --------- //
        ~AShader() = default;
};

} // namespace end
#endif /* ASHADER_H */
