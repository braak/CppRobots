cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(UNIT_TEST_TARGETS "")
macro(add_unit_test test_name target_test libraries)
  add_executable(${target_test} ${target_test}.cpp)
  target_link_libraries(${target_test} ${libraries} ${GTEST_LIBRARIES} pthread)

  add_test(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${target_test})
  set(UNIT_TEST_TARGETS ${UNIT_TEST_TARGETS} ${target_test})
endmacro(add_unit_test)

include(CTest)
find_package(GTest)
if(GTEST_FOUND AND BUILD_TESTING)
  enable_testing()

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  include_directories(${GTEST_INCLUDE_DIRS})
  # include_directories(${CMAKE_SOURCE_DIR}/src)

  add_unit_test(UnitTestVector testVector CppRobots)
  add_unit_test(UnitTestRectangle testRectangle CppRobots)
  add_unit_test(UnitTestCollision testCollision CppRobots)
  add_unit_test(UnitTestSignal testSignal CppRobots)
  add_unit_test(UnitTestMath testMath CppRobots)


  add_custom_target(run_tests
                   COMMENT "Run tests"
                   COMMAND ctest ARGS --output-on-failure
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   DEPENDS ${UNIT_TEST_TARGETS}
                 )

else(GTEST_FOUND AND BUILD_TESTING)
  message(STATUS "Unit Tests will not be generated.")
endif(GTEST_FOUND AND BUILD_TESTING)
