/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

File Name:
##  @file ICamera.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef ICAMERA_H
    #define ICAMERA_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #include "utils/utils.hpp"          // utils::vector::Vector3
    #include "../objects/AObject.hpp"   // raytracer::AObject
    #include "../rays/Ray.hpp"          // raytracer::Ray
    #include <cstdint>                  // std::uint8_t
    #include <memory>                   // std::shared_ptr
    #include <vector>                   // std::vector

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class ICamera: public raytracer::AObject {
    public:
        // ---------- Pre-Function -------- //
        virtual void init(void) = 0; // Create default rays
        virtual void reset(void) = 0; // Reset rays values (don't recreate it)
        virtual const std::vector<utils::vector::Vector3<std::uint8_t>>& getScreen(void) const = 0;
        virtual std::vector<std::shared_ptr<raytracer::Ray>> getRays(void) const = 0;
        virtual void setFieldOfView(float fieldOfView) = 0;
        virtual float getFieldOfView(void) const = 0;

        // ------------ Operator ---------- //
        ICamera& operator=(const ICamera& object) = delete;
        ICamera& operator=(ICamera&& object) = delete;

        // ---------- Constructor --------- //
        ICamera() = default;
        ICamera(const ICamera& object) = delete;
        ICamera(ICamera&& object) = delete;

        // ----------- Destructor --------- //
        ~ICamera() = default;
};

} // namespace end
#endif /* ICAMERA_H */
