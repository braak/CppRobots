cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
IF(NOT IS_DIRECTORY ${GTEST_ROOT})
  message(WARNING "GTEST_ROOT is not a valid directory. Unit Tests will not be generated.")
  return()
endif(NOT IS_DIRECTORY ${GTEST_ROOT})

enable_testing()

# include Gtest and build
subdirs(${GTEST_ROOT})
include_directories(${GTEST_ROOT}/include)
set(GTEST_LIBRARIES gtest gtest_main)

include(CTest)

macro(add_unit_test test_name target_test libraries)
  add_executable(${target_test} ${target_test}.cpp)
  target_link_libraries(${target_test} ${libraries} ${GTEST_LIBRARIES} pthread)

  add_test(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${target_test})
  set(UNIT_TEST_TARGETS ${UNIT_TEST_TARGETS} ${target_test})
endmacro(add_unit_test)

if(BUILD_TESTING)
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
endif(BUILD_TESTING)
