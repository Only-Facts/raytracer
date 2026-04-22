/**************************************************************\
Edition:
##  @date 22/04/2026 by @author Tsukini

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
        void parse(const raytracer::Raytracer& raytracer, const libconfig::Setting& node);
        void reflectRay(std::shared_ptr<raytracer::IRay> ray) const;
        float computeSDF(const utils::vector::Vector3<double>& point) const;
        utils::vector::Vector3<double> computeHit(const utils::vector::Vector3<double>& point) const;
        void kill(void) {this->_alive = false;};
        bool isAlive(void) const {return this->_alive;};

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
