/**************************************************************\
Edition:
##  @date 21/04/2026 by @author Tsukini

File Name:
##  @file DynamicLibrary.hpp

File Description:
##  You know, I don t think there are good or bad descriptions,
##  for me, life is all about functions...
\**************************************************************/

#ifndef DYNAMICLIBRARY_H
    #define DYNAMICLIBRARY_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #define _Exception
    #define _Attribute
    #include "utils/utils.hpp"  // utils::exception::CustomException, utils::exception::Type, utils::exception::Code, cold, nodiscard
    #include <cstddef>          // std::size_t
    #include <string_view>      // std::string_view
    #include <string>           // std::string
    #include <dlfcn.h>          // dlsym, dlerror

    //----------------------------------------------------------------//
    /* DEFINE */

    /* type */
    #define CAMERA 0
    #define LIGHT 1
    #define OBJECT 2
    #define MATERIAL 3

namespace raytracer { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class DynamicLibrary {
    protected:
        void* _lib = nullptr;
        std::string _path;

    public:
        std::size_t (*getType)() = nullptr; // Return the type of the lib
        const char* (*getName)() = nullptr; // Return the name of the lib

        // ------------ Function ---------- //
        nodiscard bool isloaded() const noexcept {return (this->_lib && this->getType && this->getName);};
        nodiscard std::string_view path() const noexcept {return this->_path;};
        nodiscard void* get() const noexcept {return this->_lib;};
        template<typename T>
        cold nodiscard T loadFunction(const char* name)
        {
            T func = reinterpret_cast<T>(dlsym(this->_lib, name));
            if (!func) unlikely {
                throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::Dlsym, dlerror());
            }
            return func;
        };

        // ------------ Operator ---------- //
        DynamicLibrary& operator=(const DynamicLibrary& object) = delete;
        DynamicLibrary& operator=(DynamicLibrary&& object) noexcept = default;

        // ---------- Constructor --------- //
        DynamicLibrary(const std::string& lib) noexcept;
        DynamicLibrary(const DynamicLibrary& object) = delete;
        DynamicLibrary(DynamicLibrary&& object) noexcept = default;

        // ----------- Destructor --------- //
        ~DynamicLibrary() noexcept;
};

} // namespace end
#endif /* DYNAMICLIBRARY_H */
