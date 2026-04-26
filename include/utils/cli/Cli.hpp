/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  @date 26/04/2026 by @author Tsukini

File Name:
##  @file Cli.hpp

File Description:
##  Cli class used for a customizable command line interface
\**************************************************************/

#ifndef CLI_H
    #define CLI_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "../warning/Observer.hpp"  // utils::warning::Observer
    #include "Flags.hpp"                // utils::cli::Flag, flag preset macro
    #include "Middlewares.hpp"          // utils::cli::Middlewares
    #include <termios.h>                // termios
    #include <unordered_map>            // std::unordered_map
    #include <functional>               // std::function
    #include <cstdint>                  // std::uint8_t, std::uint32_t
    #include <vector>                   // std::vector
    #include <string>                   // std::string
    #include <tuple>                    // std::tuple
    #include <queue>                    // std::queue

namespace utils::cli { // namespace start
//----------------------------------------------------------------//
/* PROTOTYPE */

// Return of the parser
using ParsedData =
std::vector< // Groupe of commands, separated by '&&', '||' and ';'
    std::vector< // Command separated by ' ' and '\t'
        // the first std::string represent only the command
        // The input parsed (depende on the parser mode)
        // the last std::string represent the separator '&&', '||', ';' or '' for nothing/last
        std::string
    >
>;

class Cli;

/* default hooks */
void defaultPromptHook(const utils::cli::Cli& cli, std::uint8_t code);
utils::cli::ParsedData defaultParserHook(const std::string& input, bool trim, bool logic, bool parse);
char defaultGetCHook();

//----------------------------------------------------------------//
/* CLASS */

class Cli: private utils::warning::Observer {
    private:
        /* global data */
        termios _orig;
        std::uint32_t _flags = utils::cli::Flags::DEFAULT;
        std::uint8_t _code = 0;
        char _inputDelimitor = '\n';
        std::queue<std::string> _initInput; // Only init

        /* storage */
        std::unordered_map<std::string, std::tuple<std::function<void(const utils::cli::Cli&, const std::vector<std::string>&)>, std::int16_t, std::int16_t>> _parsedCommands;
        std::unordered_map<std::string, std::function<void(const utils::cli::Cli&, const std::string&)>> _rawCommands;
        std::vector<std::string> _history;

        /* hooks */
        std::function<void(const utils::cli::Cli&, std::uint8_t)> _promptHook;
        std::function<utils::cli::ParsedData(const std::string&, bool, bool, bool)> _parserHook;
        std::function<char()> _getcHook;

        // ---------- Pre-Function -------- //
        void prompt();
        std::string getInput();
        utils::cli::ParsedData parse(const std::string& input);
        void exec(const utils::cli::ParsedData& parsedInput);

    public:
        /* middlewares */
        utils::cli::Middlewares<void, void> cliMiddlewares; // When the cli start & end
        utils::cli::Middlewares<std::uint8_t, std::uint8_t> errorMiddlewares; // When an error is triggered
        utils::cli::Middlewares<void, void> promptMiddlewares; // When the prompt is displayed
        utils::cli::Middlewares<void, char> inputMiddlewares; // When a key is pressed (only after is used)
        utils::cli::Middlewares<const std::string&, const utils::cli::ParsedData&> parserMiddlewares; // When the parser is called
        utils::cli::Middlewares<const utils::cli::ParsedData&, const utils::cli::ParsedData&> execMiddlewares; // When the parsed data is executed
        utils::cli::Middlewares<const std::string&, const std::string&> commandMiddlewares; // When a command is executed

        // ---------- Pre-Function -------- //
        void start(); // Stop on ctrl+d
        void start(const std::string& input); // Execute an input on start, Stop on ctrl+d
        void start(const std::vector<std::string>& inputs); // Execute multiple input on start, Stop on ctrl+d

        /* commands */
        void resetCommands();

        /* hooks */
        void resetHooks(); // Reset all hooks

        /* middlewares */
        void resetMiddlewares(); // Reset all middlewares

        /* getter */
        std::string strcode(std::uint8_t code) const;

        // ------------ Function ---------- //
        void setInputDelimitor(char c) {this->_inputDelimitor = c;};

        /* flag */
        void resetFlags() {this->_flags = utils::cli::Flags::DEFAULT;};
        void subFlags(std::uint32_t flags) {this->_flags &= ~flags;};
        void addFlags(std::uint32_t flags) {this->_flags |= flags;};
        void setFlags(std::uint32_t flags) {this->_flags = flags;};

        /* commands */
        void setCommands(const std::unordered_map<std::string, std::tuple<std::function<void(const utils::cli::Cli&, const std::vector<std::string>&)>, std::int16_t, std::int16_t>>& commands) {this->_parsedCommands = commands;}; // Set commands with min & max argument number (-1 = no limit)
        void setCommands(const std::unordered_map<std::string, std::function<void(const utils::cli::Cli&, const std::string&)>>& commands) {this->_rawCommands = commands;};

        /* hooks */
        void resetPromptHook() {this->_promptHook = defaultPromptHook;};
        void resetParserHook() {this->_parserHook = defaultParserHook;};
        void resetGetCHook() {this->_getcHook = defaultGetCHook;};
        void setPromptHook(const std::function<void(const utils::cli::Cli&, std::uint8_t)>& hook) {this->_promptHook = hook;}; // Called to print the prompt
        void setParserHook(const std::function<ParsedData(const std::string&, bool, bool, bool)>& hook) {this->_parserHook = hook;}; // Called to parse the input
        void setGetCHook(const std::function<char()>& hook) {this->_getcHook = hook;}; // Called to print the prompt

        /* getter */
        std::uint8_t getCode() const {return this->_code;};
        std::uint32_t getFlags() const {return this->_flags;};
        char getInputDelimitor() const {return this->_inputDelimitor;};
        std::vector<std::string> getHistory() const {return this->_history;};

        // ------------ Operator ---------- //
        Cli& operator=(const Cli& other) = default;
        Cli& operator=(Cli&& other) = default;

        // ---------- Constructor --------- //
        Cli();
        Cli(const Cli& other) = default;
        Cli(Cli&& other) = default;

        // ----------- Destructor --------- //
        ~Cli();
};

} // namespace end
#endif /* CLI_H */
