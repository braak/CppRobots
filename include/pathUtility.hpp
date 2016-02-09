/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file pathUtility.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __PATH__UTILITY__
#define __PATH__UTILITY__

#include <string>
#include <stdexcept>

/**
  \brief Gives the absolute Path to the current executable.
  \return the absolute Path to the current executable.
  \warning on unsupportet platforms it returns "." instead. Check your compiler
  warnings if selfpath gives you trouble.
*/
std::string selfpath();

#endif /* end of include guard: __PATH__UTILITY__ */
