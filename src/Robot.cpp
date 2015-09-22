/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Robot.cpp
*   \author Jan-Niklas Braak
*/

#include "Robot.hpp"
#include <sstream>

Robot::Robot(const double timeStep)
    : pose(), timeStep(timeStep) {}

Robot::~Robot() {}

void Robot::update(double v, double w) {
  double dx = cos(pose.theta) * v;
  double dy = sin(pose.theta) * v;
  double dTheta = w;
  Pose deltaPose(dx, dy, dTheta);
  pose += deltaPose * timeStep;
}

Pose Robot::getPose() const { return pose; }
void Robot::setPose(Pose p) {
  pose =p;
}

std::ostream& operator<<(std::ostream& os, const Robot& obj){
  os << "<Robot at " << obj.pose << ">";
  return os;
}
