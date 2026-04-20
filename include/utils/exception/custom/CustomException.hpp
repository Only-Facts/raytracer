/**************************************************************\
Edition:
##  @date 04/03/2026 by @author Tsukini

File Name:
##  @file CustomException.hpp

File Description:
##  Exception class used for custom ones
\**************************************************************/

#ifndef CUSTOMEXCEPTION_H
    #define CUSTOMEXCEPTION_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "../ExceptionDefine.hpp"           // utils::exception::Code, utils::exception::Type
    #include "../AException.hpp"                // utils::exception::AException
    #include "../../attribute/Attribute.hpp"    // cold
    #include <source_location>                  // std::source_location

namespace utils::exception { // namespace start
//----------------------------------------------------------------//
/* CLASS */

class CustomException: virtual public utils::exception::AException {
    public:
        // ------------ Operator ---------- //
        CustomException& operator=(const CustomException& object) = delete;
        CustomException& operator=(CustomException&& object) = delete;

        // ---------- Constructor --------- //
        cold CustomException(utils::exception::Type type = utils::exception::Type::None, std::string info = "[None]", std::source_location loc = std::source_location::current()) : AException(loc, type, utils::exception::Code::Undefined, info) {};
        cold CustomException(utils::exception::Type type = utils::exception::Type::None, utils::exception::Code code = utils::exception::Code::Undefined, std::string info = "[None]", std::source_location loc = std::source_location::current()) : AException(loc, type, code, info) {};
        CustomException(const CustomException& object) = delete;
        CustomException(CustomException&& object) = delete;

        // ----------- Destructor --------- //
        ~CustomException() = default;
};

} // namespace end
#endif /* CUSTOMEXCEPTION_H */
