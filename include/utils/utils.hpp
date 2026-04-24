/**************************************************************\
Edition:
##  @date 20/04/2026 by @author Tsukini

File Name:
##  @file utils.hpp

File Description:
##  Main include for every part of the utils lib
\**************************************************************/

//----------------------------------------------------------------//
/* DEFINE */

/* Activate all include */
#ifdef _Utils
    #define _Handling
    #define _Attribute
    #define _Tools
#endif

/* Activate all handling include */
#ifdef _Handling
    #define _Exception
    #define _Write
    #define _Cli
#endif

/* Activate all tool include */
#ifdef _Tools
    #define _Vector
    #define _Concepts
    #define _Algorithms
#endif

//----------------------------------------------------------------//
/* INCLUDE */

/* Exception */
#ifdef _Exception
    // -> Customized exception used for the error/warning handling
    #include "utils/exception/IException.hpp"               // utils::exception::IException
    #include "utils/exception/ExceptionDefine.hpp"          // OK/KO, utils::exception::Code, utils::exception::Type
    #include "utils/exception/basic/NoneException.hpp"      // utils::exception::NoneException
    #include "utils/exception/basic/ErrorException.hpp"     // utils::exception::ErrorException
    #include "utils/exception/basic/WarningException.hpp"   // utils::exception::WarningException
    #include "utils/exception/custom/CustomException.hpp"   // utils::exception::CustomException
#endif

/* Write */
#ifdef _Write
    // -> Handling of the ANSI escape sequences
    // -> Define some special char & other
    #include "utils/write/Color.hpp"    // utils::write::Color, utils::write::BackColor
    #include "utils/write/Char.hpp"     // utils::write::Char
    #include "utils/write/Style.hpp"    // utils::write::Style, utils::write::ResetStyle
    #include "utils/write/ANSI.hpp"     // different inline function for ANSI escape sequences
    #include "utils/write/format.hpp"   // utils::write::format
#endif

/* Cli */
#ifdef _Cli
    // -> Handling of a customizable command line interface
    #include "utils/cli/Cli.hpp"            // utils::cli::Cli, utils::cli::ParsedData
    #include "utils/cli/Flags.hpp"          // utils::cli::Flag, utils::cli::Flags
    #include "utils/cli/Middlewares.hpp"    // utils::cli::Middlewares
#endif

/* Macro */
#ifdef _Attribute
    // -> Define/Macro of attribute & keyword for optimisation & other
    #include "utils/attribute/Attribute.hpp"    // different define/macro for optimisation & other
#endif

/* VectorX */
#ifdef _Vector
    // -> Customized vector2 & vector3 for respectively 2 & 3 value of undefined type
    #include "utils/vector/IVector.hpp" // utils::vector::IVector<T>
    #include "utils/vector/Vector2.hpp" // utils::vector::Vector2<T>
    #include "utils/vector/Vector3.hpp" // utils::vector::Vector3<T>
#endif

/* Concepts */
#ifdef _Concepts
    // -> Definition of different concepts
    #include "utils/concepts/GlobalConcepts.hpp"    // Global concepts
    #include "utils/concepts/OperationConcepts.hpp" // Operation concepts
#endif

/* Algorithms */
#ifdef _Algorithms
    // -> Definition of different algorithms
    // -> c2dmp-hsm: heuristic string matching
    #include "utils/algorithms/c2dmp-hsm/c2dmp-hsm.hpp"             // utils::algorithms::c2dmp::c2dmp
    #include "utils/algorithms/c2dmp-hsm/algorithm/optimized.hpp"   // utils::algorithms::c2dmp::c2dmp_optimized
    #include "utils/algorithms/c2dmp-hsm/algorithm/simplified.hpp"  // utils::algorithms::c2dmp::c2dmp_simplified
#endif
