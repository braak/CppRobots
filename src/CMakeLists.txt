cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

add_executable(CppRobots main.cpp Robot.cpp)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
elseif(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
endif()

find_package(Git)
if(GIT_FOUND)
  execute_process(COMMAND ${GIT_EXECUTABLE} describe --long --tags --dirty --always
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE  VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)

    #parse the version information into pieces.
  string(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
  string(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
  string(REGEX REPLACE "^v[0-9]+\\.[0-9]+[-.]([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
  string(REGEX REPLACE "^v[0-9]+\\.[0-9]+[-.][0-9]+-([^-]*)(-.*)?" "\\1" VERSION_SHA1 "${VERSION}")
else()
    message(WARNING "Git not found, could not generate version information.")
  set(VERSION "unknown")
  set(VERSION_MAJOR "0")
  set(VERSION_MINOR "0")
  set(VERSION_PATCH "0")
  set(VERSION_SHA1 "deadbeef")
endif()
configure_file(${CMAKE_SOURCE_DIR}/include/version.h.in ${CMAKE_BINARY_DIR}/include/version.h)
