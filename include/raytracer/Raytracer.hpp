/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

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
    #include <unordered_map>        // std::unordered_map
    #include <cstdbool>             // bool
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
    bool viewer = false; // -ppm
    std::string ppm_path; // -ppm
    bool gui = false; // -gui
    bool modified[4] = {false}; // {camera_path, plugins_path, rendered_path, resolution}
    std::string camera_path; // -c, --camera
    std::string plugins_path = PLUGINS_PATH; // -p, --plugins
    std::string rendered_path = RENDERED_PATH; // -s, --save
    utils::vector::Vector2<std::size_t> resolution = {0, 0}; // -r, --resolution
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
        void load(int argc, char *argv[], char *env[]); // Load settings

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
