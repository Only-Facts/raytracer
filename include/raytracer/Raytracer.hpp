/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file Raytracer.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef RAYTRACER_H
    #define RAYTRACER_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Vector
    #include "utils/utils.hpp"      // utils::vector::Vector2
    #include "DynamicLibrary.hpp"   // raytracer::DynamicLibrary
    #include "cameras/ICamera.hpp"  // raytracer::ICamera
    #include "lights/ILight.hpp"    // raytracer::ILight
    #include "objects/IObject.hpp"  // raytracer::IObject
    #include <unordered_map>        // std::unordered_map
    #include <cstddef>              // std::size_t
    #include <memory>               // std::shared_ptr
    #include <vector>               // std::vector
    #include <string>               // std::string

    //----------------------------------------------------------------//
    /* DEFINE */

    /* default */
    #define PLUGINS_PATH "./plugins/"
    #define RENDERED_PATH "./rendered/"

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

struct Settings {
    bool viewer = false;
    std::string ppm_path; // <ppm_file_path>
    std::string cfg_path; // <scene_cfg_path>
    bool gui = false; // -gui
    std::string camera_path; // -c, --camera
    std::string plugins_path = PLUGINS_PATH; // -p, --plugins
    std::string rendered_path = RENDERED_PATH; // -s, --save
    utils::vector::Vector2<std::size_t> resolution = {0, 0}; // -r, --resolution

    /* edited variables */
    bool camera_set = false; // camera_path
    bool plugins_set = false; // plugins_path
    bool rendered_set = false; // rendered_path
    bool resolution_set = false; // resolution
};

class Raytracer {
    private:
        /* global data */
        raytracer::Settings _settings;

        /* plugins */
        std::unordered_map<std::string, std::shared_ptr<raytracer::DynamicLibrary>> _libs; // Keep them load until the end
        std::shared_ptr<raytracer::ICamera> _camera;
        std::vector<std::shared_ptr<raytracer::ILight>> _lights;
        std::vector<std::shared_ptr<raytracer::IObject>> _objects;

    public:
        // ---------- Pre-Function -------- //
        /* init */
        void load(int argc, char *argv[]); // Load settings

        /* gui */
        void gui(void); // Handle opening, closing of the gui
        void loop(void); // Handle loop for the gui (update the rendre or not in the viewer mode)
        void display(void); // Use the camera screen to update the gui render

        /* global */
        void render(void); // Update camera screen
        void saveRender(void); // Save the actual render to a ppm file

        // ------------ Function ---------- //
        bool isViewer(void) const {return this->_settings.viewer;};
        bool isGui(void) const {return this->_settings.gui;};

        // ------------ Operator ---------- //
        Raytracer& operator=(const Raytracer& object) = delete;
        Raytracer& operator=(Raytracer&& object) = delete;

        // ---------- Constructor --------- //
        Raytracer() = default;
        Raytracer(const Raytracer& object) = delete;
        Raytracer(Raytracer&& object) = delete;

        // ----------- Destructor --------- //
        ~Raytracer() = default;
};

} // namespace end
#endif /* RAYTRACER_H */
