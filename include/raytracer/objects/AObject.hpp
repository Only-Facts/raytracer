/**************************************************************\
Edition:
##  @date 27/04/2026 by @author Tsukini

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
    #include "../Struct.hpp"    // raytracer::CFrame, raytracer::ObjectDescriptor, raytracer::Color, raytracer::Coord, raytracer::Chunk, raytracer::ChunkObjectData
    #include "IObject.hpp"      // raytracer::IObject
    #include <unordered_map>    // std::unordered_map
    #include <vector>           // std::vector
    #include <mutex>            // std::mutex

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class AObject: public raytracer::IObject {
    private:
        mutable std::unordered_map<raytracer::Chunk, raytracer::ChunkObjectData, raytracer::ChunkHash> _lightData;
        std::mutex _lockLightData;

    protected:
        raytracer::ObjectDescriptor _descriptor;

    public:
        // ---------- Pre-Function -------- //
        /* loading */
        void loadObj(const std::string& path) final {this->loadObj(path, this->_descriptor);};
        void loadObj(const std::string& path, raytracer::ObjectDescriptor& descriptor) final;

        /* 3D logic */
        void reflectRay(raytracer::IRay* ray, const raytracer::Face* face) const final;
        std::pair<float, const raytracer::Face*> computeSDF(const raytracer::Coord& point) const override;
        raytracer::Direction computeHit(const raytracer::Coord& point, const raytracer::Face* face) const override;

        /* color handling */
        raytracer::Color getPointColor(const raytracer::Coord& point) const final;
        void addLightData(raytracer::Coord position, raytracer::Color color, float intensity) final;

        // ------------ Function ---------- //
        /* movement */
        void translate(const raytracer::Coord& v) final {this->_descriptor.cframe.position += v;};
        void rotate(const raytracer::Direction& v) final {this->_descriptor.cframe.orientation += v;};

        /* color handling */
        void clearLightData(void) final {this->_lightData.clear();};

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
