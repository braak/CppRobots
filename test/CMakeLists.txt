cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

find_package(GTest)
if(GTEST_FOUND)
  enable_testing()

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
  include_directories(${GTEST_INCLUDE_DIRS})
  include_directories(${CMAKE_SOURCE_DIR}/src)


  add_executable(testPose testPose.cpp)
  target_link_libraries(testPose ${GTEST_LIBRARIES} pthread)
  add_test(UnitTestPose testPose)

else(GTEST_FOUND)
  message(WARNING "Unit Tests will not be generated.")
endif(GTEST_FOUND)
