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
##  @file Middlewares.hpp

File Description:
##  Declaration of the Middlewares used for the cli
\**************************************************************/

#ifndef MIDDLEWARES_H
    #define MIDDLEWARES_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "../warning/Observer.hpp"                  // utils::warning::Observer
    #include "../exception/ExceptionDefine.hpp"         // utils::exception::Type, utils::exception::Code
    #include "../exception/custom/CustomException.hpp"  // utils::exception::CustomException
    #include <functional>                               // std::function
    #include <exception>                                // std::exception

namespace utils::cli { // namespace
//----------------------------------------------------------------//
/* CLASS */

template<typename T>
struct MiddlewareType {
    using type = std::function<void(T)>;
};

template<>
struct MiddlewareType<void> {
    using type = std::function<void()>;
};

template<typename T>
using Middleware = typename MiddlewareType<T>::type;

template<typename T, typename U>
class Middlewares: private utils::warning::Observer {
    public:
        std::vector<utils::cli::Middleware<T>> before;
        std::vector<utils::cli::Middleware<U>> after;

        // ------------ Function ---------- //
        void clear() {this->before.clear(); this->after.clear();}

        /* adder */
        void addBefore(utils::cli::Middleware<T>& toAdd) {this->before.push_back(toAdd);}
        void addBefore(const std::vector<utils::cli::Middleware<T>>& toAdds) {this->before.insert(before.end(), toAdds.begin(), toAdds.end());}
        void addAfter(utils::cli::Middleware<U>& toAdd) {this->after.push_back(toAdd);}
        void addAfter(const std::vector<utils::cli::Middleware<U>>& toAdds) {this->after.insert(after.end(), toAdds.begin(), toAdds.end());}

        /* caller */
        void callBefore(T arg)
        {
            for (const utils::cli::Middleware<T>& middleware: this->before) {
                try {middleware(arg);}
                catch (const std::exception& e) {throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::CliMiddleware, e.what());}
            }
        }
        void callAfter(U arg)
        {
            for (const utils::cli::Middleware<U>& middleware: this->after) {
                try {middleware(arg);}
                catch (const std::exception& e) {throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::CliMiddleware, e.what());}
            }
        }

        // ------------ Operator ---------- //
        Middlewares& operator=(const Middlewares& other) = default;
        Middlewares& operator=(Middlewares&& other) = default;

        // ---------- Constructor --------- //
        Middlewares() = default;
        Middlewares(const Middlewares& other) = default;
        Middlewares(Middlewares&& other) = default;

        // ----------- Destructor --------- //
        ~Middlewares() = default;
};

template<typename U>
class Middlewares<void, U>: private utils::warning::Observer {
    public:
        std::vector<utils::cli::Middleware<void>> before;
        std::vector<utils::cli::Middleware<U>> after;

        // ------------ Function ---------- //
        void clear() {this->before.clear(); this->after.clear();}

        /* adder */
        void addBefore(utils::cli::Middleware<void>& toAdd) {this->before.push_back(toAdd);}
        void addBefore(const std::vector<utils::cli::Middleware<void>>& toAdds) {this->before.insert(before.end(), toAdds.begin(), toAdds.end());}
        void addAfter(utils::cli::Middleware<U>& toAdd) {this->after.push_back(toAdd);}
        void addAfter(const std::vector<utils::cli::Middleware<U>>& toAdds) {this->after.insert(after.end(), toAdds.begin(), toAdds.end());}

        /* caller */
        void callBefore()
        {
            for (const utils::cli::Middleware<void>& middleware: this->before) {
                try {middleware();}
                catch (const std::exception& e) {throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::CliMiddleware, e.what());}
            }
        }
        void callAfter(U arg)
        {
            for (const utils::cli::Middleware<U>& middleware: this->after) {
                try {middleware(arg);}
                catch (const std::exception& e) {throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::CliMiddleware, e.what());}
            }
        }

        // ------------ Operator ---------- //
        Middlewares& operator=(const Middlewares& other) = default;
        Middlewares& operator=(Middlewares&& other) = default;

        // ---------- Constructor --------- //
        Middlewares() = default;
        Middlewares(const Middlewares& other) = default;
        Middlewares(Middlewares&& other) = default;

        // ----------- Destructor --------- //
        ~Middlewares() = default;
};

template<typename T>
class Middlewares<T, void>: private utils::warning::Observer {
    public:
        std::vector<utils::cli::Middleware<T>> before;
        std::vector<utils::cli::Middleware<void>> after;

        // ------------ Function ---------- //
        void clear() {this->before.clear(); this->after.clear();}

        /* adder */
        void addBefore(utils::cli::Middleware<T>& toAdd) {this->before.push_back(toAdd);}
        void addBefore(const std::vector<utils::cli::Middleware<T>>& toAdds) {this->before.insert(before.end(), toAdds.begin(), toAdds.end());}
        void addAfter(utils::cli::Middleware<void>& toAdd) {this->after.push_back(toAdd);}
        void addAfter(const std::vector<utils::cli::Middleware<void>>& toAdds) {this->after.insert(after.end(), toAdds.begin(), toAdds.end());}

        /* caller */
        void callBefore(T arg)
        {
            for (const utils::cli::Middleware<T>& middleware: this->before) {
                try {middleware(arg);}
                catch (const std::exception& e) {throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::CliMiddleware, e.what());}
            }
        }
        void callAfter()
        {
            for (const utils::cli::Middleware<void>& middleware: this->after) {
                try {middleware();}
                catch (const std::exception& e) {throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::CliMiddleware, e.what());}
            }
        }

        // ------------ Operator ---------- //
        Middlewares& operator=(const Middlewares& other) = default;
        Middlewares& operator=(Middlewares&& other) = default;

        // ---------- Constructor --------- //
        Middlewares() = default;
        Middlewares(const Middlewares& other) = default;
        Middlewares(Middlewares&& other) = default;

        // ----------- Destructor --------- //
        ~Middlewares() = default;
};

template<>
class Middlewares<void, void>: private utils::warning::Observer {
    public:
        std::vector<utils::cli::Middleware<void>> before;
        std::vector<utils::cli::Middleware<void>> after;

        // ------------ Function ---------- //
        void clear() {this->before.clear(); this->after.clear();}

        /* adder */
        void addBefore(utils::cli::Middleware<void>& toAdd) {this->before.push_back(toAdd);}
        void addBefore(const std::vector<utils::cli::Middleware<void>>& toAdds) {this->before.insert(before.end(), toAdds.begin(), toAdds.end());}
        void addAfter(utils::cli::Middleware<void>& toAdd) {this->after.push_back(toAdd);}
        void addAfter(const std::vector<utils::cli::Middleware<void>>& toAdds) {this->after.insert(after.end(), toAdds.begin(), toAdds.end());}

        /* caller */
        void callBefore()
        {
            for (const utils::cli::Middleware<void>& middleware: this->before) {
                try {middleware();}
                catch (const std::exception& e) {throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::CliMiddleware, e.what());}
            }
        }
        void callAfter()
        {
            for (const utils::cli::Middleware<void>& middleware: this->after) {
                try {middleware();}
                catch (const std::exception& e) {throw utils::exception::CustomException(utils::exception::Type::Error, utils::exception::Code::CliMiddleware, e.what());}
            }
        }

        // ------------ Operator ---------- //
        Middlewares& operator=(const Middlewares& other) = default;
        Middlewares& operator=(Middlewares&& other) = default;

        // ---------- Constructor --------- //
        Middlewares() = default;
        Middlewares(const Middlewares& other) = default;
        Middlewares(Middlewares&& other) = default;

        // ----------- Destructor --------- //
        ~Middlewares() = default;
};

} // namespace end
#endif /* MIDDLEWARES_H */
