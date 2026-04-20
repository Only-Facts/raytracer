/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

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
    #include "utils/utils.hpp"  // utils::vector::Vector3
    #include "../Struct.hpp"    // raytracer::CFrame
    #include <memory>           // std::shared_ptr

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class IRay;

class IObject {
    public:
        // ---------- Pre-Function -------- //
        virtual void reflectRay(std::shared_ptr<raytracer::IRay> ray) const = 0;
        virtual float computeSDF(const utils::vector::Vector3<double>& point) const = 0; // Return the closest norm with the point
        virtual void translate(const utils::vector::Vector3<double>& v) = 0; // Apply the translation on the object
        virtual void addLightRay(std::pair<utils::vector::Vector3<double>, float> lightRay) = 0;
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
