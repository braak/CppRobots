/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Follower.cpp
*   \author Jan-Niklas Braak
*/

#include "Agents/Follower.hpp"

Follower::Follower(double target_distance, double K_distance, double K_angle)
    : target_distance(target_distance), K_distance(K_distance),
      K_angle(K_angle) {}

Action Follower::update(Robot const &r) {
  auto target_robot = r.scanClosest();

  if (!target_robot) {
    // If no visible Robot, turn in circle.
    // NOTE: for faster turning, we could also turn our turret, but we'd have to
    // be carefull not to overshoot. Also navigating to the target gets a bit
    // more complex that way.
    const double w = -r.rules.scan_angle / r.rules.timeStep;
    return {0, w, 0, false};
  }

  // get our position and rotation.
  const auto position = r.getPosition();
  const auto rotation = r.getRotation();

  // get our targets position
  const auto target_position = target_robot->getPosition();
  // calculate vector from our position to the target.
  const auto diff = target_position - position;

  // calculate the error value of our distance to desired distance.
  const auto distance_error = diff.magnitude() - target_distance;
  // calculate the error value of our rotation to desired rotation.
  const auto angle_error = angDiffRadians(diff.angle(), rotation);

  // Apply simple proportional controllers to our errors. Using K_distance and
  // K_angle as gain
  const auto v = K_distance * distance_error;
  const auto w = K_angle * angle_error;

  return {v, w, 0, false};
}
