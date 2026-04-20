/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

File Name:
##  @file IRay.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef IRAY_H
    #define IRAY_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "../objects/AObject.hpp"   // raytracer::AObject

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class IRay: public raytracer::AObject {
    public:
        // ---------- Pre-Function -------- //
        virtual void reset(void) = 0; // Reset ray values

        // ------------ Operator ---------- //
        IRay& operator=(const IRay& object) = delete;
        IRay& operator=(IRay&& object) = delete;

        // ---------- Constructor --------- //
        IRay() = default;
        IRay(const IRay& object) = delete;
        IRay(IRay&& object) = delete;

        // ----------- Destructor --------- //
        virtual ~IRay() = default;
};

} // namespace end
#endif /* IRAY_H */
