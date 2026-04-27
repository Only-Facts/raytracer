/**************************************************************\
Edition:
##  @date 27/04/2026 by @author Tsukini

File Name:
##  @file ACamera.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef ACAMERA_H
    #define ACAMERA_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #define _Attribute
    #include "utils/utils.hpp"  // utils::vector::Vector3, utils::vector::Vector2, nodiscard
    #include "ICamera.hpp"      // raytracer::ICamera
    #include "../Define.hpp"    // values
    #include "../rays/Ray.hpp"  // raytracer::Ray
    #include <cstdint>          // std::uint8_t, std::uint16_t
    #include <memory>           // std::shared_ptr
    #include <vector>           // std::vector

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class ACamera: public raytracer::ICamera {
    protected:
        std::vector<utils::vector::Vector3<std::uint8_t>> _screen; // Screen pixel
        std::vector<raytracer::Ray*> _rays;
        utils::vector::Vector2<std::uint16_t> _resolution;

    public:
        // ---------- Pre-Function -------- //
        void updateScreen(void) final;

        // ------------ Function ---------- //
        nodiscard const std::vector<utils::vector::Vector3<std::uint8_t>>& getScreen(void) const {return this->_screen;};
        nodiscard std::vector<raytracer::Ray*> getRays(void) const {return this->_rays;};
        void setResolution(utils::vector::Vector2<std::uint16_t> resolution) {this->_resolution = resolution;};
        nodiscard utils::vector::Vector2<std::uint16_t> getResolution(void) const {return this->_resolution;};

        // ------------ Operator ---------- //
        ACamera& operator=(const ACamera& object) = delete;
        ACamera& operator=(ACamera&& object) = delete;

        // ---------- Constructor --------- //
        ACamera() = default;
        ACamera(const ACamera& object) = delete;
        ACamera(ACamera&& object) = delete;

        // ----------- Destructor --------- //
        ~ACamera();
};

} // namespace end
#endif /* ACAMERA_H */
