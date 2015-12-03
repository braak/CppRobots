cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package(GTest)
if(GTEST_FOUND)
  enable_testing()

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
  include_directories(${GTEST_INCLUDE_DIRS})
  include_directories(${CMAKE_SOURCE_DIR}/src)
  #include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  #set(GTEST_LIBRARIES "lib/libgtest.a lib/libgtest_main.a")

  add_executable(testVector testVector.cpp)
  target_link_libraries(testVector CppRobots ${GTEST_LIBRARIES} pthread)
  add_test(UnitTestVector testVector)

  add_executable(testRectangle testRectangle.cpp)
  target_link_libraries(testRectangle CppRobots ${GTEST_LIBRARIES} pthread)
  add_test(UnitTestRectangle testRectangle)

  add_executable(testCollision testCollision.cpp)
  target_link_libraries(testCollision CppRobots ${GTEST_LIBRARIES} pthread)
  add_test(UnitTestCollision testCollision)

  add_executable(testSignal testSignal.cpp)
  target_link_libraries(testSignal CppRobots ${GTEST_LIBRARIES} pthread)
  add_test(UnitTestSignal testSignal)

else(GTEST_FOUND)
  message(WARNING "Unit Tests will not be generated.")
endif(GTEST_FOUND)
