/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file robots.cpp
*   \author Jan-Niklas Braak
*/

#include <iostream>

#include "CppRobots.hpp"

#include "Agents/Orbiter.hpp"

/**
    This is the main function of the program.
    \return returns 0 on success
*/
int main() {
  const double timeBase = 1.0 / 60;
  Robot robot(timeBase);
  Agent *agent = new Orbiter(20, 0.6);
  std::cout << "Welcome to CppRobots v" << VERSION_SHORT << std::endl;

  for (size_t i = 0; i < 1000; i++) {
    std::cout << robot << std::endl;
    Robot::Action action = agent->update(robot);
    robot.update(action);
  }
  return 0;
}
