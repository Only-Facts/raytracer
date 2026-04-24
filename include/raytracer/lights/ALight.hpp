/**************************************************************\
Edition:
##  @date 24/04/2026 by @author Tsukini

File Name:
##  @file ALight.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef ALIGHT_H
    #define ALIGHT_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Attribute
    #include "utils/utils.hpp"      // nodiscard
    #include "ILight.hpp"           // raytracer::ILight
    #include "../rays/LightRay.hpp" // raytracer::LightRay
    #include "../Define.hpp"        // values
    #include <cstdint>              // std::uint8_t   
    #include <limits>               // UINT8_MAX

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class ALight: public raytracer::ILight {
    protected:
        std::vector<raytracer::LightRay*> _rays;
        utils::vector::Vector3<std::uint8_t> _color = DEFAULT_COLOR;
        float _intensity = 1.0f;

    public:
        // ------------ Function ---------- //
        std::vector<raytracer::LightRay*> getRays(void) const {return this->_rays;};

        // ------------ Operator ---------- //
        ALight& operator=(const ALight& object) = delete;
        ALight& operator=(ALight&& object) = delete;

        // ---------- Constructor --------- //
        ALight() = default;
        ALight(const ALight& object) = delete;
        ALight(ALight&& object) = delete;

        // ----------- Destructor --------- //
        ~ALight();
};

} // namespace
#endif /* ALIGHT_H */
