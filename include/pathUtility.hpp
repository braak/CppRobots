/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file pathUtility.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __PATH__UTILITY__
#define __PATH__UTILITY__

#ifdef __linux__

#include <unistd.h>
#include <limits.h>
#include <libgen.h>

#else
/* NOTE: for OS X see
https://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man3/dyld.3.html

for Windows see https://msdn.microsoft.com/en-us/library/ms683197.aspx
 */

#warning Platform not supported by pathUtility
#endif

#include <string>
#include <stdexcept>

std::string selfpath();

#endif /* end of include guard: __PATH__UTILITY__ */
