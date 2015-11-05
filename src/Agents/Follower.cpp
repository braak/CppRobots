/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Follower.cpp
*   \author Jan-Niklas Braak
*/

#include "Agents/Follower.hpp"

Follower::Follower(double target_distance, double K_distance, double K_beta)
    : target_distance(target_distance), K_distance(K_distance), K_beta(K_beta) {
}

Action Follower::update(Robot const &r) {
  std::shared_ptr<Robot> target_robot = r.scanClosest();

  if (!target_robot) {
    // If no visible Robot, turn in circle.
    // NOTE: for faster turning, we could also turn our turret, but we'd have to
    // be carefull not to overshoot. Also navigating to the target gets a bit
    // more complex that way.
    return {0, -r.rules.scan_angle / r.rules.timeStep, 0, false};
  }

  Vector_d position = r.getPosition();
  double rotation = r.getRotation();

  Vector_d target_position = target_robot->getPosition();
  Vector_d diff = position - target_position;
  const double distance_error = diff.magnitude() - target_distance;
  const double angle_error = angDiffRadians(rotation, diff.angle());
  return {K_distance * distance_error, K_beta * angle_error, 0, false};
}
