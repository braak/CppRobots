/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Robot.cpp
*   \author Jan-Niklas Braak
*/

#include "Robot.hpp"
#include <sstream>

Robot::Robot(const double timeStep) : pose(), timeStep(timeStep) {}

Pose Robot::getPose() const { return pose; }
void Robot::setPose(Pose p) { pose = p; }

void Robot::update(Action const &a) {
  double dx = cos(pose.theta) * a.v;
  double dy = sin(pose.theta) * a.v;
  double dTheta = a.w;
  Pose deltaPose(dx, dy, dTheta);
  pose += deltaPose * timeStep;
}

std::ostream &operator<<(std::ostream &os, const Robot &obj) {
  os << "<Robot at " << obj.pose << ">";
  return os;
}
