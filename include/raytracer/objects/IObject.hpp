/**************************************************************\
Edition:
##  @date 27/04/2026 by @author Tsukini

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
    #include "utils/utils.hpp"              // utils::vector::Vector3
    #include "../materials/IMaterial.hpp"   // raytracer::IMaterial
    #include "../Struct.hpp"                // raytracer::CFrame, raytracer::ObjectDescriptor, raytracer::Color
    #include <libconfig.h++>                // libconfig::Setting
    #include <cstdint>                      // std::uint8_t
    #include <memory>                       // std::shared_ptr
    #include <tuple>                        // std::tuple

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class IRay;
class Raytracer;

class IObject {
    public:
        // ---------- Pre-Function -------- //
        /* loading */
        virtual void parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node) = 0;
        virtual void loadObj(const std::string& path) = 0;
        virtual void loadObj(const std::string& path, raytracer::ObjectDescriptor& descriptor) = 0;

        /* 3D logic */
        virtual void reflectRay(raytracer::IRay* ray) const = 0;
        virtual float computeSDF(const raytracer::Coord& point) const = 0;
        virtual raytracer::Coord computeHit(const raytracer::Coord& point) const = 0;

        /* movement */
        virtual void translate(const raytracer::Coord& v) = 0;
        virtual void rotate(const raytracer::Coord& v) = 0;

        /* color handling */
        virtual raytracer::Color getPointColor(const raytracer::Coord& point) const = 0;
        virtual void addLightRay(raytracer::Coord position, raytracer::Color color, float intensity) = 0;
        virtual void clearLightRays(void) = 0;

        /* getter & setter */
        virtual void setObjectDescriptor(const raytracer::ObjectDescriptor& descriptor) = 0;
        virtual void setCFrame(const raytracer::CFrame& cframe) = 0;
        virtual const raytracer::ObjectDescriptor& getObjectDescriptor(void) const = 0;
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
