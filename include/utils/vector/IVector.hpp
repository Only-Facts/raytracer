/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  @date 08/04/2026 by @author Tsukini

File Name:
##  @file IVector.hpp

File Description:
##  Interface for the cutomized vector
\**************************************************************/

#ifndef IVECTOR_H
    #define IVECTOR_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include <cstddef>  // std::size_t

namespace utils::vector { // namespace start
//----------------------------------------------------------------//
/* CLASS */

template<typename T>
class IVector {
    public:
        // ---------- Pre-Function -------- //
        virtual T get(std::size_t index) const = 0;

        // ------------ Operator ---------- //
        IVector& operator=(const IVector& object) = default;
        IVector& operator=(IVector&& object) = default;

        // ---------- Constructor --------- //
        IVector() = default;
        IVector(const IVector& object) = default;
        IVector(IVector&& object) = default;

        // ----------- Destructor --------- //
        virtual ~IVector() = default;
};

} // namespace end
#endif /* IVECTOR_H */
