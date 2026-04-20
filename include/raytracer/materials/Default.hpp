/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

File Name:
##  @file Default.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef DEFAULT_H
    #define DEFAULT_H

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

class Default: public raytracer::AMaterial {
    public:
        // ------------ Operator ---------- //
        Default& operator=(const Default& object) = delete;
        Default& operator=(Default&& object) = delete;

        // ---------- Constructor --------- //
        Default() = default;
        Default(const Default& object) = delete;
        Default(Default&& object) = delete;

        // ----------- Destructor --------- //
        ~Default() = default;
};

} // namespace end
#endif /* DEFAULT_H */
