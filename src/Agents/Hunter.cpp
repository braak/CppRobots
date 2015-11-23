/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Hunter.cpp
*   \author Jan-Niklas Braak
*/

#include "Agents/Hunter.hpp"
#include "Angles.hpp"
#include "math.h"

#include <iostream>

Hunter::Hunter(double targetDistance, double K_beta, double K_distance)
    : targetDistance(targetDistance), K_beta(K_beta), K_distance(K_distance) {}

Action Hunter::update(Robot const &r) {

  double turretAngle;
  Vector_d deltaPosition;

  std::shared_ptr<Robot> targetRobot = r.scanClosest();
  Vector_d position = r.getPosition();
  double rotation = r.getRotation();
  // TODO: seperate movement and shooting code. Add Wall avoidance.
  if (!targetRobot) {
    Vector_d targetPosition = r.rules.arena_size / 2.0;

    deltaPosition = targetPosition - position;

    turretAngle = r.getTurretAngle() + r.rules.scan_angle;
  } else {
    Vector_d targetPosition = targetRobot->getPosition();

    deltaPosition = targetPosition - position;

    turretAngle = deltaPosition.angle() - rotation;
  }

  Vector_d perp = Vector_d(deltaPosition).rotate(M_PI / 2);
  // turn perpandicular to the target
  const double beta_error = angDiffRadians(perp.angle(), rotation);
  // turn toward the target
  const double angle_error = angDiffRadians(deltaPosition.angle(), rotation);
  // How far are we away from the target, as a value from -1 to 1.
  const double distance_error = std::min(
      (deltaPosition.magnitude() - targetDistance) / targetDistance, 1.0);

  const double w = distance_error * angle_error * K_distance +
                   (1 - distance_error) * beta_error * K_beta;

  bool shooting;
  if (abs(angDiffRadians(turretAngle, r.getTurretAngle())) < 0.01 &&
      targetRobot) {
    shooting = true;
  } else {
    shooting = false;
  }
  return {r.rules.v_max, w, turretAngle, shooting};
}
