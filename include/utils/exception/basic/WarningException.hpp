/**************************************************************\
Edition:
##  @date 03/03/2026 by @author Tsukini

File Name:
##  @file WarningException.hpp

File Description:
##  Exception class used for Warning
\**************************************************************/

#ifndef WARNINGEXCEPTION_H
    #define WARNINGEXCEPTION_H

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

class WarningException: virtual public utils::exception::AException {
    public:
        // ------------ Operator ---------- //
        WarningException& operator=(const WarningException& object) = delete;
        WarningException& operator=(WarningException&& object) = delete;

        // ---------- Constructor --------- //
        cold WarningException(utils::exception::Code code = utils::exception::Code::Undefined, std::source_location loc = std::source_location::current()) : AException(loc, utils::exception::Type::Warning, code) {};
        WarningException(const WarningException& object) = delete;
        WarningException(WarningException&& object) = delete;

        // ----------- Destructor --------- //
        ~WarningException() = default;
};

} // namespace end
#endif /* WARNINGEXCEPTION_H */
