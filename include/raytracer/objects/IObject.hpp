/**************************************************************\
Edition:
##  @date 22/04/2026 by @author Tsukini

File Name:
##  @file IObject.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef IOBJECT_H
    #define IOBJECT_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #include "utils/utils.hpp"          // utils::vector::Vector3
    #include "../Struct.hpp"            // raytracer::CFrame
    #include <libconfig.h++>            // libconfig::Setting
    #include <cstdint>                  // std::uint8_t
    #include <memory>                   // std::shared_ptr
    #include <tuple>                    // std::tuple

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class IRay;
class Raytracer;

class IObject {
    public:
        // ---------- Pre-Function -------- //
        virtual void parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node) = 0;
        virtual void reflectRay(std::shared_ptr<raytracer::IRay> ray) const = 0;
        virtual float computeSDF(const utils::vector::Vector3<double>& point) const = 0;
        virtual utils::vector::Vector3<double> computeHit(const utils::vector::Vector3<double>& point) const = 0;
        virtual void translate(const utils::vector::Vector3<double>& v) = 0;
        virtual void rotate(const utils::vector::Vector3<double>& v) = 0;
        virtual utils::vector::Vector3<std::uint8_t> getPointColor(const utils::vector::Vector3<double>& point) const = 0;
        virtual void addLightRay(std::tuple<utils::vector::Vector3<double>, utils::vector::Vector3<std::uint8_t>, float> lightRay) = 0;
        virtual void clearLightRays(void) = 0;
        virtual void setShapeDescriptor(const raytracer::ShapeDescriptor& descriptor) = 0;
        virtual void setCFrame(const raytracer::CFrame& cframe) = 0;
        virtual const raytracer::ShapeDescriptor& getShapeDescriptor(void) const = 0;
        virtual raytracer::CFrame getCFrame(void) const = 0;

        // ------------ Operator ---------- //
        IObject& operator=(const IObject& object) = delete;
        IObject& operator=(IObject&& object) = delete;

        // ---------- Constructor --------- //
        IObject() = default;
        IObject(const IObject& object) = delete;
        IObject(IObject&& object) = delete;

        // ----------- Destructor --------- //
        virtual ~IObject() = default;
};

} // namespace end
#endif /* IOBJECT_H */
