/**************************************************************\
Edition:
##  @date 27/04/2026 by @author Tsukini

File Name:
##  @file DynamicLibrary.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#define _Exception
#define _Attribute
#include "utils/utils.hpp"
#include "raytracer/DynamicLibrary.hpp"
#include <iostream>
#include <cstddef>
#include <string_view>
#include <string>

cold raytracer::DynamicLibrary::DynamicLibrary(const std::string& lib) noexcept
{
    this->_lib = dlopen(lib.c_str(), RTLD_NOW);
    this->_path = lib;
    if (!(this->_lib)) unlikely {
        utils::exception::CustomException e(utils::exception::Type::Error, utils::exception::Code::Dlopen, dlerror());
        std::cout << e.formated() << std::endl;
        return;
    }

    // Load type getter
    try {
        this->getType = this->loadFunction<std::size_t (*)()>("type");
    } catch (const utils::exception::IException& e) {
        std::cout << e.formated() << std::endl;
        return;
    }

    // Load name getter
    try {
        this->getName = this->loadFunction<const char* (*)()>("name");
    } catch (const utils::exception::IException& e) {
        std::cout << e.formated() << std::endl;
        return;
    }
}

cold raytracer::DynamicLibrary::~DynamicLibrary() noexcept
{
    if (this->_lib && dlclose(this->_lib) != OK) unlikely {
        utils::exception::CustomException e(utils::exception::Type::Error, utils::exception::Code::Dlclose, dlerror());
        std::cout << e.formated() << std::endl;
    }
}
