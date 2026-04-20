/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

File Name:
##  @file RGB.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef RGB_H
    #define RGB_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #define _Attribute
    #include "utils/utils.hpp"  // utils::vector::Vector3, nodiscard
    #include "AMaterial.hpp"    // raytracer:AMaterial    
    #include <vector>           // std::vector

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class RGB: public raytracer::AMaterial {
    public:
        // ------------ Operator ---------- //
        RGB& operator=(const RGB& object) = delete;
        RGB& operator=(RGB&& object) = delete;

        // ---------- Constructor --------- //
        RGB() = default;
        RGB(const RGB& object) = delete;
        RGB(RGB&& object) = delete;

        // ----------- Destructor --------- //
        ~RGB() = default;
};

} // namespace end
#endif /* RGB_H */
