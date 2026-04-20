/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  @date 12/04/2026 by @author Tsukini

File Name:
##  @file Flags.hpp

File Description:
##  Definition of the flags used to customize the cli
\**************************************************************/

#ifndef CLIFLAGS_H
    #define CLIFLAGS_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include <cstdint>  // std::uint32_t

namespace utils::cli { // namespace
//----------------------------------------------------------------//
/* CLASS */

enum Flag {
    DEBUG           = 1 << 0, // Active verbose for internal action
    NOECHO          = 1 << 1, // Disable echo of the input
    CATCH           = 1 << 2, // Enable error catching on execution
    EMPTY_INPUT     = 1 << 3, // Ingore empty input (default: error)
    TRIM            = 1 << 4, // Enable trim on input
    PARSED          = 1 << 5, // Active parser for the input and send vector<std::string> (default: std::string)
    PROMPT          = 1 << 6, // Active the prompt
    LOGIC           = 1 << 7, // Enable logic with '&&', '||' and ';'
    ARROW           = 1 << 8, // Activate left, right
    HISTORY         = 1 << 9, // Activate history, up and down arrow
    HINT            = 1 << 10, // Display hint when a command fail
    AUTO_COMPLETION = 1 << 11, // Active auto completion with `\t` (only work on the first command for now)
};

/*
// ------------ Operator ---------- //
constexpr utils::cli::Flag operator|(utils::cli::Flag lhs, utils::cli::Flag rhs)
{return static_cast<utils::cli::Flag>(static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs));}
constexpr utils::cli::Flag operator&(utils::cli::Flag lhs, utils::cli::Flag rhs)
{return static_cast<utils::cli::Flag>(static_cast<std::uint32_t>(lhs) & static_cast<std::uint32_t>(rhs));}
constexpr utils::cli::Flag operator^(utils::cli::Flag lhs, utils::cli::Flag rhs)
{return static_cast<utils::cli::Flag>(static_cast<std::uint32_t>(lhs) ^ static_cast<std::uint32_t>(rhs));}
constexpr utils::cli::Flag operator~(utils::cli::Flag f)
{return static_cast<utils::cli::Flag>(~static_cast<std::uint32_t>(f));}

inline utils::cli::Flag& operator|=(utils::cli::Flag& lhs, utils::cli::Flag rhs) {return lhs = lhs | rhs;}
inline utils::cli::Flag& operator&=(utils::cli::Flag& lhs, utils::cli::Flag rhs) {return lhs = lhs & rhs;}
inline utils::cli::Flag& operator^=(utils::cli::Flag& lhs, utils::cli::Flag rhs) {return lhs = lhs ^ rhs;}
*/
} // namespace end

namespace utils::cli::Flags { // namespace
//----------------------------------------------------------------//
/* MACRO */

/* flags preset */
constexpr std::uint32_t DEFAULT = CATCH | EMPTY_INPUT | TRIM | PROMPT | ARROW;
constexpr std::uint32_t DUMB    = 0;
constexpr std::uint32_t ALL     = DEBUG | CATCH | NOECHO | TRIM | EMPTY_INPUT | PARSED | PROMPT | LOGIC | ARROW | HISTORY | HINT | AUTO_COMPLETION;
constexpr std::uint32_t TERM1   = CATCH | EMPTY_INPUT | TRIM | PARSED | PROMPT | EMPTY_INPUT | LOGIC | ARROW | HISTORY;
constexpr std::uint32_t TERM2   = TERM1 | HINT | AUTO_COMPLETION;
constexpr std::uint32_t DEV     = TERM2 | DEBUG;

} // namespace end
#endif /* CLIFLAGS_H */
