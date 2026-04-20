/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

File Name:
##  @file AObject.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef AOBJECT_H
    #define AOBJECT_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #define _Attribute
    #include "utils/utils.hpp"  // utils::vector::Vector3, nodiscard
    #include "../Struct.hpp"    // raytracer::CFrame, raytracer::ShapeDescriptor
    #include "IObject.hpp"      // raytracer::IObject
    #include <vector>           // std::vector

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class AObject: public raytracer::IObject {
    protected:
        raytracer::ShapeDescriptor _descriptor;
        std::vector<std::pair<utils::vector::Vector3<double>, float>> _lightRays;

    public:
        // ------------ Function ---------- //
        void translate(const utils::vector::Vector3<double>& v) {this->_descriptor.cframe.position += v;};
        void rotate(const utils::vector::Vector3<double>& v) {this->_descriptor.cframe.orientation += v;};
        void addLightRay(std::pair<utils::vector::Vector3<double>, float> lightRay) {this->_lightRays.push_back(lightRay);};
        void setShapeDescriptor(const raytracer::ShapeDescriptor& descriptor) {this->_descriptor = descriptor;};
        void setCFrame(const raytracer::CFrame& cframe) {this->_descriptor.cframe = cframe;};
        nodiscard const raytracer::ShapeDescriptor& getShapeDescriptor(void) const {return this->_descriptor;};
        nodiscard raytracer::CFrame getCFrame(void) const {return this->_descriptor.cframe;};

        // ------------ Operator ---------- //
        AObject& operator=(const AObject& object) = delete;
        AObject& operator=(AObject&& object) = delete;

        // ---------- Constructor --------- //
        AObject() = default;
        AObject(const AObject& object) = delete;
        AObject(AObject&& object) = delete;

        // ----------- Destructor --------- //
        ~AObject() = default;
};

} // namespace end
#endif /* AOBJECT_H */
