/**************************************************************\
Edition:
##  @date 11/04/2026 by @author Tsukini

File Name:
##  @file ANSI.hpp

File Description:
##  Definition of ANSI escape sequences
\**************************************************************/

#ifndef ANSI_H
    #define ANSI_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "Color.hpp"        // utils::write::Color, utils::write::BackColor
    #include "Char.hpp"         // utils::write::Char
    #include "Style.hpp"        // utils::write::Style, utils::write::ResetStyle
    #include <initializer_list> // std::initializer_list
    #include <cstdint>          // std::uint8_t, std::size_t
    #include <format>           // std::format
    #include <string>           // std::string

namespace utils::write { // namespace start
    //----------------------------------------------------------------//
    /* ANSI */

    /* --------- function ---------- */
    std::string setStyle(std::initializer_list<utils::write::Style> styles);
    std::string resetStyle(std::initializer_list<utils::write::ResetStyle> styles);

    /* ----------- reset ----------- */
    constexpr inline std::string reset()                    {return std::format("{}[0m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string strong_reset()             {return std::format("{}[21m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string dark_reset()               {return std::format("{}[22m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string italic_reset()             {return std::format("{}[23m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string underlined_reset()         {return std::format("{}[24m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string flashing_fast_reset()      {return std::format("{}[25m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string flashing_slow_reset()      {return std::format("{}[26m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string reversed_reset()           {return std::format("{}[27m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string hide_reset()               {return std::format("{}[28m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string bar_reset()                {return std::format("{}[29m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string framed_encircled_reset()   {return std::format("{}[54m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string overlined_reset()          {return std::format("{}[55m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string underline_color_reset()    {return std::format("{}[59m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string exposant_indice_reset()    {return std::format("{}[75m", static_cast<char>(utils::write::Char::ESC));}
    /* args */
    constexpr inline std::string resetStyle(utils::write::ResetStyle style) {return std::format("{}[{}m", static_cast<char>(utils::write::Char::ESC), static_cast<std::uint8_t>(style));}

    /* ----------- style ----------- */
    constexpr inline std::string strong()           {return std::format("{}[1m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string dark()             {return std::format("{}[2m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string italic()           {return std::format("{}[3m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string underlined()       {return std::format("{}[4m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string flashing_fast()    {return std::format("{}[5m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string flashing_slow()    {return std::format("{}[6m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string reversed()         {return std::format("{}[7m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string hide()             {return std::format("{}[8m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string bar()              {return std::format("{}[9m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string monospace()        {return std::format("{}[50m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string framed()           {return std::format("{}[51m", static_cast<char>(utils::write::Char::ESC));} // Rarely supported
    constexpr inline std::string encircled()        {return std::format("{}[52m", static_cast<char>(utils::write::Char::ESC));} // Rarely supported
    constexpr inline std::string overlined()        {return std::format("{}[53m", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string exposant()         {return std::format("{}[73m", static_cast<char>(utils::write::Char::ESC));} // Rarely supported
    constexpr inline std::string indice()           {return std::format("{}[74m", static_cast<char>(utils::write::Char::ESC));} // Rarely supported
    /* args */
    constexpr inline std::string setStyle(utils::write::Style style)                                    {return std::format("{}[{}m", static_cast<char>(utils::write::Char::ESC), static_cast<std::uint8_t>(style));}
    constexpr inline std::string color(utils::write::Color c)                                           {return std::format("{}[{}m", static_cast<char>(utils::write::Char::ESC), static_cast<std::uint8_t>(c));}
    constexpr inline std::string color(utils::write::BackColor c)                                       {return std::format("{}[{}m", static_cast<char>(utils::write::Char::ESC), static_cast<std::uint8_t>(c));}
    constexpr inline std::string color_id(std::uint8_t id)                                              {return std::format("{}[38;5;{}m", static_cast<char>(utils::write::Char::ESC), id);}
    constexpr inline std::string back_color_id(std::uint8_t id)                                         {return std::format("{}[48;5;{}m", static_cast<char>(utils::write::Char::ESC), id);}
    constexpr inline std::string underline_color_id(std::uint8_t id)                                    {return std::format("{}[58;5;{}m", static_cast<char>(utils::write::Char::ESC), id);}
    constexpr inline std::string color_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b)              {return std::format("{}[38;2;{};{};{}m", static_cast<char>(utils::write::Char::ESC), r, g, b);}
    constexpr inline std::string back_color_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b)         {return std::format("{}[48;2;{};{};{}m", static_cast<char>(utils::write::Char::ESC), r, g, b);}
    constexpr inline std::string underline_color_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b)    {return std::format("{}[58;2;{};{};{}m", static_cast<char>(utils::write::Char::ESC), r, g, b);}

    /* ---------- cursor ----------- */
    constexpr inline std::string load_cur() {return std::format("{}[u", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string save_cur() {return std::format("{}[s", static_cast<char>(utils::write::Char::ESC));}
    /* args */
    constexpr inline std::string up(std::size_t n)                      {return std::format("{}[{}A", static_cast<char>(utils::write::Char::ESC), n);}
    constexpr inline std::string down(std::size_t n)                    {return std::format("{}[{}B", static_cast<char>(utils::write::Char::ESC), n);}
    constexpr inline std::string right(std::size_t n)                   {return std::format("{}[{}C", static_cast<char>(utils::write::Char::ESC), n);}
    constexpr inline std::string left(std::size_t n)                    {return std::format("{}[{}D", static_cast<char>(utils::write::Char::ESC), n);}
    constexpr inline std::string next_line(std::size_t n)               {return std::format("{}[{}E", static_cast<char>(utils::write::Char::ESC), n);}
    constexpr inline std::string previous_line(std::size_t n)           {return std::format("{}[{}F", static_cast<char>(utils::write::Char::ESC), n);}
    constexpr inline std::string column(std::size_t col)                {return std::format("{}[{}G", static_cast<char>(utils::write::Char::ESC), col);}
    constexpr inline std::string pos(std::size_t row, std::size_t col)  {return std::format("{}[{};{}H", static_cast<char>(utils::write::Char::ESC), row, col);}
    constexpr inline std::string scroll_up(std::size_t n)               {return std::format("{}[{}S", static_cast<char>(utils::write::Char::ESC), n);}
    constexpr inline std::string scroll_down(std::size_t n)             {return std::format("{}[{}T", static_cast<char>(utils::write::Char::ESC), n);}

    /* ----------- erase ----------- */
    constexpr inline std::string screen_end()           {return std::format("{}[0J", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string screen_start()         {return std::format("{}[1J", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string screen()               {return std::format("{}[2J", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string scrollback_buffer()    {return std::format("{}[3J", static_cast<char>(utils::write::Char::ESC));} // Can delete the term history
    constexpr inline std::string line_end()             {return std::format("{}[0K", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string line_start()           {return std::format("{}[1K", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string line()                 {return std::format("{}[2K", static_cast<char>(utils::write::Char::ESC));}

    /* ------ private-modes ------- */
    constexpr inline std::string inverted_color_enable()    {return std::format("{}[?5h", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string inverted_color_disable()   {return std::format("{}[?5l", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string wrapping_enable()          {return std::format("{}[?7h", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string wrapping_disable()         {return std::format("{}[?7l", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string show_cur()                 {return std::format("{}[?25h", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string hide_cur()                 {return std::format("{}[?25l", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string save_screen()              {return std::format("{}[?1049h", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string load_screen()              {return std::format("{}[?1049l", static_cast<char>(utils::write::Char::ESC));}
    
    /* --------- reports ---------- */
    constexpr inline std::string get_pos()                      {return std::format("{}[6n", static_cast<char>(utils::write::Char::ESC));} // Reports cursor position as "ESC[row;colR"
    constexpr inline std::string mouse_tracking_enable()        {return std::format("{}[?1000h", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string mouse_tracking_disable()       {return std::format("{}[?1000l", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string mouse_move_tracking_enable()   {return std::format("{}[?1002h", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string mouse_move_tracking_disable()  {return std::format("{}[?1002l", static_cast<char>(utils::write::Char::ESC));}
    // Reports mouse action as (each x&y value as a '+ 32') "ESC[Mb;x;y" -> 'b' is the boutton & modifier
    // Boutton: 0 left, 1 mid, 2 right, 3 release
    // Modifer: +4 shift, +8 alt, +16 ctrl
    constexpr inline std::string mouse_adv_tracking_enable()    {return std::format("{}[?1006h", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string mouse_adv_tracking_disable()   {return std::format("{}[?1006l", static_cast<char>(utils::write::Char::ESC));}
    // Reports mouse action as "ESC[<b;x;y(M|m)" -> 'b' is the boutton & modifier -> M = pressed, m = released
    // Boutton: 0 left, 1 mid, 2 right, 3 release
    // Modifer: +4 shift, +8 alt, +16 ctrl
    constexpr inline std::string report_focus_enable()          {return std::format("{}[?1004h", static_cast<char>(utils::write::Char::ESC));} // Focus in: "ESC[I" | Focus out: "ESC[O"
    constexpr inline std::string report_focus_disabled()        {return std::format("{}[?1004l", static_cast<char>(utils::write::Char::ESC));}
    constexpr inline std::string report_past_enable()           {return std::format("{}[?2004h", static_cast<char>(utils::write::Char::ESC));} // Reports for pasted data: "ESC[200~{data}ESC[201~"
    constexpr inline std::string report_past_disable()          {return std::format("{}[?2004l", static_cast<char>(utils::write::Char::ESC));}

//----------------------------------------------------------------//
/* TYPDEF */

/* mousse button */
enum class MouseButton {
    Left,
    Right,
    Middle,
    Release,
    Unknown
};

/* mouse event -> "ESC[Mb;x;y" */
struct MouseEvent {
    utils::write::MouseButton button = utils::write::MouseButton::Unknown;
    std::size_t x = 0;
    std::size_t y = 0;
};

/* mouse event -> "ESC[<b;x;y(M|m)" */
struct AdvancedMouseEvent {
    utils::write::MouseButton button = utils::write::MouseButton::Unknown;
    std::size_t x = 0;
    std::size_t y = 0;
    bool pressed = false;
};

/* reports */
std::pair<int, int> readCursorPosition();
utils::write::MouseEvent readMouseEvent();
utils::write::AdvancedMouseEvent readAdvancedMouseEvent();

} // namespace ends
#endif /* ANSI_H */
