/**************************************************************\
Edition:
##  @date 25/04/2026 by @author Tsukini

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
    #define _Exception
    #define _Vector
    #define _Attribute
    #include "utils/utils.hpp"          // utils::exception::*, utils::vector::Vector2, nodiscard
    #include "DynamicLibrary.hpp"       // raytracer::DynamicLibrary
    #include "Struct.hpp"               // raytracer::ObjectDescriptor, raytracer::Color, raytracer::HugeColor
    #include "Define.hpp"               // values
    #include "special/Sky.hpp"          // raytracer::Sky
    #include "cameras/ICamera.hpp"      // raytracer::ICamera
    #include "materials/IMaterial.hpp"  // raytracer::IMaterial
    #include "lights/ILight.hpp"        // raytracer::ILight
    #include "objects/IObject.hpp"      // raytracer::IObject
    #include <SFML/Graphics.hpp>        // sf::RenderWindow
    #include <libconfig.h++>            // libconfig::Setting
    #include <unordered_map>            // std::unordered_map
    #include <cstddef>                  // std::size_t
    #include <memory>                   // std::shared_ptr
    #include <vector>                   // std::vector
    #include <string>                   // std::string

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

struct Settings {
    bool viewer = false;
    std::string ppm_path; // <ppm_file_path>
    std::string cfg_path; // <scene_cfg_path>
    bool gui = false; // -gui
    bool newton = false; // -n, --newton (unused for now)
    std::string camera_path; // -c, --camera
    std::string plugins_path = PLUGINS_PATH; // -p, --plugins
    std::string rendered_path = RENDERED_PATH; // -s, --save
    std::string obj_path = OBJ_PATH; // -o, --obj
    utils::vector::Vector2<std::size_t> resolution = {0, 0}; // -r, --resolution

    /* edited variables */
    bool camera_set = false; // camera_path
    bool plugins_set = false; // plugins_path
    bool rendered_set = false; // rendered_path
    bool obj_set = false; // obj_path
    bool resolution_set = false; // resolution
};

class Raytracer {
    private:
        /* global data */
        raytracer::Settings _settings;

        /* plugins */
        std::unordered_map<std::string, std::shared_ptr<raytracer::DynamicLibrary>> _libs; // Keep them load until the end
        raytracer::Sky _sky;
        raytracer::ICamera* _camera = nullptr;
        std::vector<raytracer::ILight*> _lights;
        std::vector<raytracer::IObject*> _objects;

        // ------------ Function ---------- //
        template<typename T>
        T* factory(const std::string& name) const
        {
            auto it = this->_libs.find(name);
            if (it == this->_libs.end())
                throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::NoPlugins, std::string("Invalid type in the scene, can't find corresponding plugin: ") + name);
            std::shared_ptr<raytracer::DynamicLibrary> lib = it->second;
            T* (*fn)() = lib->loadFunction<T* (*)()>("factory");
            return fn();
        }

    public:
        // ---------- Pre-Function -------- //
        /* init */
        void load(int argc, char *argv[]); // Load settings
        void init(void); // Init plugins & rays

        /* parsing */
        void scene(void); // Load scene file
        void parseSceneFile(const std::string& path, std::string& content) const; // Parse the cfg file for custom tokens
        raytracer::IMaterial* parseMaterial(const libconfig::Setting& node) const;
        void parseLight(const libconfig::Setting& node);
        void parseObject(const libconfig::Setting& node);

        /* gui */
        void gui(void); // Handle opening, closing of the gui
        void loop(sf::RenderWindow& window); // Handle loop for the gui (multithreaded, update the render, not in the viewer mode)
        void display(sf::RenderWindow& window); // Use the camera screen to update the gui render

        /* global */
        void render(void); // Update camera screen
        void loadRender(void); // Load the given ppm file
        void saveRender(void); // Save the actual render to a ppm file

        // ------------ Function ---------- //
        raytracer::ICamera* getCamera(void) const {return this->_camera;};
        bool isViewer(void) const {return this->_settings.viewer;};
        bool isGui(void) const {return this->_settings.gui;};
        std::string ObjPath(const std::string& path) const {return this->_settings.obj_path + path;};

        // -------- Static-Function ------- //
        static inline nodiscard raytracer::Color mergeColor(raytracer::HugeColor color1, raytracer::HugeColor color2, float intensity = 1.0f)
        {return color1 * color2 * intensity / 255;}

        // ------------ Operator ---------- //
        Raytracer& operator=(const Raytracer& object) = delete;
        Raytracer& operator=(Raytracer&& object) = delete;

        // ---------- Constructor --------- //
        Raytracer() = default;
        Raytracer(const Raytracer& object) = delete;
        Raytracer(Raytracer&& object) = delete;

        // ----------- Destructor --------- //
        ~Raytracer();
};

} // namespace end
#endif /* RAYTRACER_H */
