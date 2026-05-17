/**************************************************************\
Edition:
##  @date 17/05/2026 by @author Tsukini

File Name:
##  @file IShader.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef ISHADER_H
    #define ISHADER_H

    //----------------------------------------------------------------//
    /* INCLUDE */
    
    /* type */
    #define _Vector
    #include "utils/utils.hpp"              // utils::vector::Vector3
    #include "../Struct.hpp"                // raytracer::* (types)
    #include "../rays/LightRay.hpp"         // raytracer::LightRay
    #include "../materials/IMaterial.hpp"   // raytracer::IMaterial
    #include <libconfig.h++>                // libconfig::Setting

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class IShader {
    public:
        // ---------- Pre-Function -------- //
        virtual void parse(const libconfig::Setting& node) = 0;
        virtual raytracer::Color screen(const raytracer::Coord2D& point, const raytracer::Color& pixel) = 0;
        virtual raytracer::Color scene(const raytracer::Coord& worldPoint, const raytracer::Coord& localPoint, const raytracer::Color& color) = 0;
        virtual bool light(raytracer::LightRay* ray, raytracer::IObject* object, float sdf) = 0; // Can overite the light behavior
        virtual void material(raytracer::IMaterial* material) = 0;

        // ------------ Operator ---------- //
        IShader& operator=(const IShader& object) = delete;
        IShader& operator=(IShader&& object) = delete;

        // ---------- Constructor --------- //
        IShader() = default;
        IShader(const IShader& object) = delete;
        IShader(IShader&& object) = delete;

        // ----------- Destructor --------- //
        virtual ~IShader() = default;
};

} // namespace end
#endif /* ISHADER_H */
