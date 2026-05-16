/**************************************************************\
Edition:
##  @date 13/05/2026 by @author Tsukini

File Name:
##  @file CameraFOV.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef CAMERA_H
    #define CAMERA_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "ACamera.hpp"  // raytracer::ACamera

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class CameraFOV: public raytracer::ACamera {
    private:
        float _fieldOfView = 70.0f;

    public:
        // ---------- Pre-Function -------- //
        void parse(const libconfig::Setting& node) final;
        void init(void) final;
        void reset(void) final;
        //void setFieldOfView(float fieldOfView) {this->_fieldOfView = fieldOfView;};
        //nodiscard float getFieldOfView(void) const {return this->_fieldOfView;};

        // ------------ Operator ---------- //
        CameraFOV& operator=(const CameraFOV& object) = delete;
        CameraFOV& operator=(CameraFOV&& object) = delete;

        // ---------- Constructor --------- //
        CameraFOV() = default;
        CameraFOV(const CameraFOV& object) = delete;
        CameraFOV(CameraFOV&& object) = delete;

        // ----------- Destructor --------- //
        ~CameraFOV() = default;
};

} // namespace end
#endif /* CAMERA_H */
