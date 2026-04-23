/**************************************************************\
Edition:
##  @date 23/04/2026 by @author Tsukini

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
    #include "../Struct.hpp"    // raytracer::CFrame, raytracer::ObjectDescriptor
    #include "IObject.hpp"      // raytracer::IObject
    #include <vector>           // std::vector

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class AObject: public raytracer::IObject {
    private:
        std::vector<std::tuple<utils::vector::Vector3<double>, utils::vector::Vector3<std::uint8_t>, float>> _lightRays;

    protected:
        raytracer::ObjectDescriptor _descriptor;

    public:
        // ---------- Pre-Function -------- //
        /* loading */
        void loadObj(const std::string& path) final {this->loadObj(path, this->_descriptor);};
        void loadObj(const std::string& path, raytracer::ObjectDescriptor& descriptor) final;

        /* 3D logic */
        void reflectRay(std::shared_ptr<raytracer::IRay> ray) const final;
        float computeSDF(const utils::vector::Vector3<double>& point) const final;
        utils::vector::Vector3<double> computeHit(const utils::vector::Vector3<double>& point) const final;

        /* color handling */
        utils::vector::Vector3<std::uint8_t> getPointColor(const utils::vector::Vector3<double>& point) const final;

        // ------------ Function ---------- //
        /* movement */
        void translate(const utils::vector::Vector3<double>& v) final {this->_descriptor.cframe.position += v;};
        void rotate(const utils::vector::Vector3<double>& v) final {this->_descriptor.cframe.orientation += v;};

        /* color handling */
        void addLightRay(std::tuple<utils::vector::Vector3<double>, utils::vector::Vector3<std::uint8_t>, float> lightRay) final {this->_lightRays.push_back(lightRay);};
        void clearLightRays(void) final {this->_lightRays.clear();};

        /* getter & setter */
        void setObjectDescriptor(const raytracer::ObjectDescriptor& descriptor) final {this->_descriptor = descriptor;};
        void setCFrame(const raytracer::CFrame& cframe) final {this->_descriptor.cframe = cframe;};
        nodiscard const raytracer::ObjectDescriptor& getObjectDescriptor(void) const final {return this->_descriptor;};
        nodiscard raytracer::CFrame getCFrame(void) const final {return this->_descriptor.cframe;};

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
