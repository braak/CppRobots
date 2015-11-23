/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file pathUtility.cpp
*   \author Jan-Niklas Braak
*/

#include "pathUtility.hpp"

std::string selfpath() {
#ifdef __linux__
  char buff[PATH_MAX];
  ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);
  if (len != -1) {
    buff[len] = '\0';
    return std::string(dirname(buff));
  }
  throw std::runtime_error("unable to find executable path");
#endif
  // This may be right.
  return ".";
}
