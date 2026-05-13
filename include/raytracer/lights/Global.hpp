/**************************************************************\
Edition:
##  @date 13/05/2026 by @author Tsukini

File Name:
##  @file Global.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef GLOBAL_H
    #define GLOBAL_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "ALight.hpp"   // raytracer::ALight

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class Global: public raytracer::ALight {
    public:
        // ---------- Pre-Function -------- //
        void parse(const libconfig::Setting& node) final;
        void init(void) final;
        void reset(void) final;

        // ------------ Operator ---------- //
        Global& operator=(const Global& object) = delete;
        Global& operator=(Global&& object) = delete;

        // ---------- Constructor --------- //
        Global() = default;
        Global(const Global& object) = delete;
        Global(Global&& object) = delete;

        // ----------- Destructor --------- //
        ~Global() = default;
};

} // namespace end
#endif /* GLOBAL_H */
