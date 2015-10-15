/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Robot.cpp
*   \author Jan-Niklas Braak
*/

#include "Robot.hpp"
#include <sstream>

Robot::Robot(const double timeStep) : timeStep(timeStep) {}

Pose Robot::getPose() const { return pose; }
void Robot::setPose(Pose p) { pose = p; }

void Robot::update(Action const &a) {
  const double v = std::max(std::min(a.v, v_max), v_min);
  const double w = std::max(std::min(a.w, w_max), -w_max);

  const double dx = cos(pose.theta) * v;
  const double dy = sin(pose.theta) * v;
  const double dTheta = w;
  Pose deltaPose(dx, dy, dTheta);
  pose += deltaPose * timeStep;
}

void Robot::setScanTargets(std::list<std::shared_ptr<Robot>> scanTargets_) {
  scanTargets = scanTargets_;
}

std::list<std::shared_ptr<Robot>> Robot::getScanTargets() const {
  return scanTargets;
}

std::ostream &operator<<(std::ostream &os, const Robot &obj) {
  os << "<Robot at " << obj.pose << ">";
  return os;
}
