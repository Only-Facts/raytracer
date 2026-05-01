/**************************************************************\
Edition:
##  @date 27/04/2026 by @author Tsukini

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
        raytracer::Color _color = DEFAULT_COLOR;
        float _intensity = 1.0f;
        bool _global = false;

    public:
        // ------------ Function ---------- //
        nodiscard std::vector<raytracer::LightRay*> getRays(void) const {return this->_rays;};
        nodiscard raytracer::Color getColor(void) const {return this->_color;};
        nodiscard float getIntensity(void) const {return this->_intensity;};
        void enableGlobal(void) {this->_global = true;};
        nodiscard bool isGlobal(void) const {return this->_global;};

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
