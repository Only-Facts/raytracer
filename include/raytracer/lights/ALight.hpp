/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

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

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class ALight: public raytracer::ILight {
    protected:
        std::vector<std::shared_ptr<raytracer::LightRay>> _rays;
        bool _global = false;

    public:
        // ---------- Pre-Function -------- //
        void reflectRay(std::shared_ptr<raytracer::IRay> ray) const;
        float computeSDF(const utils::vector::Vector3<double>& point) const;

        // ------------ Function ---------- //
        std::vector<std::shared_ptr<raytracer::LightRay>>& getRays(void) const {return this->_rays;};
        void setGlobal(bool global) {this->_global = global;};
        nodiscard bool isGlobal(void) const {return this->_global;};

        // ------------ Operator ---------- //
        ALight& operator=(const ALight& object) = delete;
        ALight& operator=(ALight&& object) = delete;

        // ---------- Constructor --------- //
        ALight() = default;
        ALight(const ALight& object) = delete;
        ALight(ALight&& object) = delete;

        // ----------- Destructor --------- //
        ~ALight() = default;
};

} // namespace
#endif /* ALIGHT_H */
