/**************************************************************\
Edition:
##  @date 27/04/2026 by @author Tsukini

File Name:
##  @file ARay.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef ARAY_H
    #define ARAY_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "../objects/AObject.hpp"   // raytracer::AObject
    #include "../Define.hpp"            // values
    #include "IRay.hpp"                 // raytracer::IRay

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class ARay: public raytracer::IRay {
    protected:
        bool _alive = true;

    public:
        // ---------- Pre-Function -------- //
        void parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node) final;

        // ------------ Function ---------- //
        void kill(void) final {this->_alive = false;};
        bool isAlive(void) const final {return this->_alive;};

        // ------------ Operator ---------- //
        ARay& operator=(const ARay& object) = delete;
        ARay& operator=(ARay&& object) = delete;

        // ---------- Constructor --------- //
        ARay() = default;
        ARay(const ARay& object) = delete;
        ARay(ARay&& object) = delete;

        // ----------- Destructor --------- //
        ~ARay() = default;
};

} // namespace end
#endif /* ARAY_H */
