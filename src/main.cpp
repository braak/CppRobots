/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file main.cpp
*   \author Jan-Niklas Braak
*/

#include <iostream>
#include "version.h"
#include "Robot.hpp"

/**
    This is the main function of the program.
    \param argc number of commandline parameters
    \param argv array of command line parameters
    \return returns 0 on success
*/
int main(int argc, char const *argv[]) {
  const double timeBase = 1.0 / 60;
  Robot robot(timeBase);
  std::cout << "Welcome to CppRobots " << VERSION << std::endl;

  for (size_t i = 0; i < 100; i++) {
    std::cout << (std::string)robot << std::endl;
    robot.update(1, 0.0000001);
  }
  return 0;
}
