/**************************************************************\
Edition:
##  @date 24/04/2026 by @author Tsukini

File Name:
##  @file Sky.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef SKY_H
    #define SKY_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Attribute
    #include "utils/utils.hpp"  // nodiscard
    #include "../Define.hpp"    // DEFAULT_COLOR
    #include "../Struct.hpp"    // raytracer::Color 

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Sky {
    private:
        raytracer::Color _color = DEFAULT_COLOR;

    public:
        // ------------ Function ---------- //
        void setColor(raytracer::Color color) {this->_color = color;};
        nodiscard raytracer::Color getColor() const {return this->_color;};

        // ------------ Operator ---------- //
        Sky& operator=(const Sky& object) = delete;
        Sky& operator=(Sky&& object) = delete;

        // ---------- Constructor --------- //
        Sky() = default;
        Sky(const Sky& object) = delete;
        Sky(Sky&& object) = delete;

        // ----------- Destructor --------- //
        ~Sky() = default;
};

} // namespace end
#endif /* SKY_H */
