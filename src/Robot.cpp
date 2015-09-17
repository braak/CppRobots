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
  // double w = max(min(w, -M_PI / 2), M_PI / 2);
  double dx = cos(theta) * v;
  double dy = sin(theta) * v;
  double dTheta = w; // v / 100 * tan(w);

  x = x + dx * timeBase;
  y = y + dy * timeBase;
  theta = theta + dTheta * timeBase;
}

Pose Robot::getPose() { return Pose(x, y, theta); }
void Robot::setPose(Pose p) {
  x = p.x;
  y = p.y;
  theta = p.theta;
}
Robot::operator std::string() {
  std::stringstream str;
  str << "<Robot at " << x << ", " << y << ", " << theta << ">";
  return str.str();
}
