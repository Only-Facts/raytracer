/**************************************************************\
Edition:
##  @date 17/05/2026 by @author Tsukini

File Name:
##  @file AMaterial.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef AMATERIAL_H
    #define AMATERIAL_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #define _Attribute
    #include "utils/utils.hpp"  // utils::vector::Vector3, nodiscard
    #include "IMaterial.hpp"    // raytracer:IMaterial
    #include "../Define.hpp"    // values
    #include <limits>           // UINT8_MAX
    #include <vector>           // std::vector

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class AMaterial: public raytracer::IMaterial {
    protected:
        raytracer::Color _color = DEFAULT_COLOR; // Material color

        /* surface */
        float _transparency = 0.0f; // Material transparency
        float _reflection = 0.0f; // Material reflection

    public:
        // ---------- Pre-Function -------- //
        void parse(const libconfig::Setting& node) override;

        // ------------ Function ---------- //
        nodiscard raytracer::Color getColor(void) const final {return this->_color;};
        nodiscard float getTransparency(void) const final {return this->_transparency;};
        nodiscard float getReflection(void) const final {return this->_reflection;};

        /* used by material shader */
        void setColor(const raytracer::Color& color) final {this->_color = color;};
        void setTransparency(float transparency) final {this->_transparency = transparency;};
        void setReflection(float reflection) final {this->_reflection = reflection;};

        // ------------ Operator ---------- //
        AMaterial& operator=(const AMaterial& object) = delete;
        AMaterial& operator=(AMaterial&& object) = delete;

        // ---------- Constructor --------- //
        AMaterial() = default;
        AMaterial(const AMaterial& object) = delete;
        AMaterial(AMaterial&& object) = delete;

        // ----------- Destructor --------- //
        ~AMaterial() = default;
};

} // namespace end
#endif /* AMATERIAL_H */
