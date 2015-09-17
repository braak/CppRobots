/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Robot.cpp
*   \author Jan-Niklas Braak
*/

#include "Robot.hpp"
#include <sstream>

Robot::Robot(const double timeBase)
    : x(0), y(0), theta(0), timeBase(timeBase) {}

Robot::~Robot() {}

void Robot::update(double v, double w) {
  //   TODO: limit speed and acceleration
  double dx = sin(theta) * v;
  double dy = cos(theta) * v;
  double dTheta = w;

  x = x + dx * timeBase;
  y = y + dy * timeBase;
  theta = theta + dTheta * timeBase;
}

Robot::operator std::string() {
  std::stringstream str;
  str << "<Robot at " << x << ", " << y << ", " << theta << ">";
  return str.str();
}
