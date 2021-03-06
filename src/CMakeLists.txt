cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# A library used by all programs
set(CPP_ROBOTS_LIBRARIES FrameTimer.cpp Robot.cpp Simulation.cpp Vector.cpp Rectangle.cpp Collision.cpp Projectile.cpp Rules.cpp Game.cpp ViewConsole.cpp pathUtility.cpp jsoncpp.cpp)
add_library(CppRobots ${CPP_ROBOTS_LIBRARIES})

# A library, that contains all Agents
file(GLOB AGENT_SOURCES
    "Agents/*.cpp"
)
add_library(CppRobotsAgents ${AGENT_SOURCES})
target_link_libraries(CppRobotsAgents CppRobots)

add_executable(robots robots.cpp)

# find out if SFML can be used
find_package(SFML 2 COMPONENTS system window graphics)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})

    # The library used by programms using SFML
    add_library(CppRobotsSFML ViewSFML.cpp)

    target_link_libraries(CppRobotsSFML ${SFML_LIBRARIES})

    # create the executable for use with UI
    target_link_libraries(robots ${SFML_LIBRARIES} CppRobots CppRobotsSFML CppRobotsAgents)

    # copy all rescource files to the runtime directory
    file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    add_definitions(-DUSE_SFML)
else(SFML_FOUND)
  # create the executable for use with Console
  target_link_libraries(robots CppRobots CppRobotsAgents)

  message(WARNING "robots will not be build with UI support, because SFML was not found.")
endif(SFML_FOUND)

file(COPY ${CMAKE_SOURCE_DIR}/config DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

find_package(Git)
if(GIT_FOUND)
  execute_process(COMMAND ${GIT_EXECUTABLE} describe --long --tags --dirty --always
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE  VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  string(REGEX REPLACE "^v([0-9]+)\\.([0-9]+)[-.]([0-9]+)-([^-]*)(-.*)?" "\\1;\\2;\\3;\\4" VERSION_LIST "${VERSION}")
  list(GET VERSION_LIST 0 VERSION_MAJOR)
  list(GET VERSION_LIST 1 VERSION_MINOR)
  list(GET VERSION_LIST 2 VERSION_PATCH)
  list(GET VERSION_LIST 3 VERSION_SHA1)
  # list(GET VERSION_LIST 4 VERSION_POSTFIX)

else()
  message(WARNING "Git not found, could not generate version information.")
  set(VERSION "unknown")
  set(VERSION_MAJOR "0")
  set(VERSION_MINOR "0")
  set(VERSION_PATCH "0")
  set(VERSION_SHA1 "")
endif()
message(STATUS "Current repository version is ${VERSION}")
configure_file(${CMAKE_SOURCE_DIR}/cmake/version.h.in ${CMAKE_BINARY_DIR}/include/version.h)
