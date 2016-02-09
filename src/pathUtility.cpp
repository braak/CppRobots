/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file pathUtility.cpp
*   \author Jan-Niklas Braak
*/

#include "pathUtility.hpp"

#ifdef __linux__

#include <unistd.h>
#include <libgen.h>
#include <climits>
#include <cerrno>
#include <cstring>

std::string selfpath() {
  char buff[PATH_MAX];
  ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff) - 1);
  if (len != -1) {
    buff[len] = '\0';
    return std::string(dirname(buff));
  }
  throw std::runtime_error("Unable to find executable path :" +
                           std::string(std::strerror(errno)));
}
#elif _WINDOWS
#include <windows.h>
#include "Shlwapi.h"

#pragma comment(lib, "shlwapi.lib")
std::string selfpath() {
  WCHAR path[MAX_PATH];
  GetModuleFileNameW(NULL, path, MAX_PATH);
  PathRemoveFileSpecW(path);

  std::wstring ws(path);
  return std::string(ws.begin(), ws.end());
}

#else
/* NOTE: for OS X see
https://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man3/dyld.3.html
 */
#warning Platform not supported by pathUtility. Behaviour of functions may not be as expected.

std::string selfpath() {
  // This is correct some of the time.
  return ".";
}
#endif
