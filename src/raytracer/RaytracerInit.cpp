/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file RaytracerInit.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/Raytracer.hpp"
#include <filesystem>
#include <exception>

static void isDirectory(const std::string& path)
{
    if (!std::filesystem::is_directory(path))
        throw utils::exception::ErrorException(utils::exception::Code::InvalidFile);
}

static void isFile(const std::string& path, const std::string& extensionWanted = "")
{
    if (!std::filesystem::is_regular_file(path))
        throw utils::exception::ErrorException(utils::exception::Code::InvalidFile);

    // Check extension
    if (extensionWanted.empty()) return;
    if (std::filesystem::path(path).extension() != extensionWanted) {
        utils::exception::CustomException e(utils::exception::Warning, utils::exception::Code::InvalidFileExtension, std::string("Excepted extension: ") + extensionWanted + ", got: " + std::filesystem::path(path).extension().string());
        std::cout << e.formated() << std::endl;
    }
}

void raytracer::Raytracer::load(int argc, char *argv[])
{
    const char* envVar = nullptr;
    int i = 1;

    // Check for a minimum of arguments
    if (argc < 2) unlikely {
        throw utils::exception::ErrorException(utils::exception::Code::ArgumentsNumber);
    }

    // Handle first argument <scene_cfg_path> || <ppm_file_path>
    if (std::filesystem::path(argv[i]).extension() != ".cfg"
        && std::filesystem::path(argv[i]).extension() != ".ppm") {
        if (argc > 2) { // cfg
            isFile(argv[i]);
            utils::exception::CustomException e(utils::exception::Warning, utils::exception::Code::InvalidFileExtension, "The first file given was interpreted has a '.cfg'");
            std::cout << e.formated() << std::endl;
            this->_settings.viewer = false;
            this->_settings.cfg_path = argv[i];
        } else { // ppm
            isFile(argv[i]);
            utils::exception::CustomException e(utils::exception::Warning, utils::exception::Code::InvalidFileExtension, "The first file given was interpreted has a '.ppm'");
            std::cout << e.formated() << std::endl;
            this->_settings.viewer = true;
            this->_settings.ppm_path = argv[i];
            return;
        }
    } else if (std::filesystem::path(argv[i]).extension() == ".cfg") {
        isFile(argv[i], ".cfg");
        this->_settings.viewer = false;
        this->_settings.cfg_path = argv[i];
    } else {
        isFile(argv[i], ".ppm");
        this->_settings.viewer = true;
        this->_settings.ppm_path = argv[i];
        return;
    }
    ++i;

    // Is in gui mode?
    if (argc > i && std::string(argv[i]) == "-gui") {
        this->_settings.gui = true;
        ++i;
    }

    // Global argument handling
    std::string arg;
    for (; i < argc; ++i) {
        arg = std::string(argv[i]);

        // -c, --camera <used_camera_path>
        if (arg == "-c" || arg == "--camera") {
            if (i + 1 >= argc)
                throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::MissingOptionArgument, "-c requires <used_camera_path>");

            // Check the option argument
            if (this->_settings.camera_set) {
                utils::exception::WarningException e(utils::exception::Code::OptionOverride);
                std::cout << e.formated() << std::endl;
            }
            isFile(argv[++i], ".so");

            // Set the path to the camera used
            this->_settings.camera_set = true;
            this->_settings.camera_path = argv[i];
        }

        // -p, --plugins <plugins_directory_path>
        else if (arg == "-p" || arg == "--plugins") {
            if (i + 1 >= argc)
                throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::MissingOptionArgument, "-p requires <plugins_directory_path>");

            // Check the option argument
            if (this->_settings.plugins_set) {
                utils::exception::WarningException e(utils::exception::Code::OptionOverride);
                std::cout << e.formated() << std::endl;
            }
            isDirectory(argv[++i]);

            // Set the plugins search path
            this->_settings.plugins_set = true;
            this->_settings.plugins_path = argv[i];
        }

        // -s, --save <ppm_directory_path>
        else if (arg == "-s" || arg == "--save") {
            if (i + 1 >= argc)
                throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::MissingOptionArgument, "-s requires <ppm_directory_path>");

            // Check the option argument
            if (this->_settings.rendered_set) {
                utils::exception::WarningException e(utils::exception::Code::OptionOverride);
                std::cout << e.formated() << std::endl;
            }
            isDirectory(argv[++i]);

            // Set the ppm save path
            this->_settings.rendered_set = true;
            this->_settings.rendered_path = argv[++i];
        }

        // -r, --resolution "wxh"
        else if (arg == "-r" || arg == "--resolution") {
            if (i + 1 >= argc)
                throw utils::exception::CustomException(utils::exception::Error, utils::exception::Code::MissingOptionArgument, "-r requires \"wxh\"");

            // Check the option argument
            if (this->_settings.resolution_set) {
                utils::exception::WarningException e(utils::exception::Code::OptionOverride);
                std::cout << e.formated() << std::endl;
            }
            std::string res = argv[++i];
            auto pos = res.find('x');
            if (pos == std::string::npos)
                throw utils::exception::CustomException(utils::exception::Warning, utils::exception::Code::InvalidArgument, std::string("Invalid resolution format: ") + res);

            // Try to set the resolution
            try {
                this->_settings.resolution.x = std::stoul(res.substr(0, pos));
                this->_settings.resolution.y = std::stoul(res.substr(pos + 1));
            } catch (const std::exception& e) {
                throw utils::exception::CustomException(utils::exception::Warning, utils::exception::Code::InvalidArgument, std::string("Invalid resolution format: ") + res);
            }
            this->_settings.resolution_set = true;
        }

        // Ignored argument
        else {
            utils::exception::CustomException e(utils::exception::Warning, utils::exception::Code::InvalidArgument, arg);
            std::cout << e.formated() << std::endl;
        }
    }

    // Try env var getting if the arguments din't already set it
    if (!this->_settings.plugins_set) {
        if ((envVar = std::getenv("RAYTRACER_PLUGINS_PATH"))) {
            this->_settings.plugins_path = envVar;
        } /*else {
            utils::exception::CustomException e(utils::exception::Warning, utils::exception::Code::MissingEnvVar, "Wasn't able to find the environement variable: RAYTRACER_PLUGINS_PATH");
            std::cout << e.formated() << std::endl;
        }*/
    }
    if (!this->_settings.rendered_set) {
        if ((envVar = std::getenv("RAYTRACER_RENDERED_PATH"))) {
            this->_settings.rendered_path = envVar;
        } /*else {
            utils::exception::CustomException e(utils::exception::Warning, utils::exception::Code::MissingEnvVar, "Wasn't able to find the environement variable: RAYTRACER_RENDERED_PATH");
            std::cout << e.formated() << std::endl;
        }*/
    }
}
