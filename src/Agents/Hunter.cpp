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
  // const double targetDistance = 100;
  // const double K_beta = 100;
  // const double K_distance = 30;

  std::shared_ptr<Robot> targetRobot = r.scanClosest();

  if (!targetRobot) {
    std::cout << "Hunter: No Target" << std::endl;
    return {0, 0, r.getTurretAngle() + r.rules.scan_angle, false};
  }

  Vector_d position = r.getPosition();
  double rotation = r.getRotation();

  Vector_d targetPosition = targetRobot->getPosition();
  Vector_d deltaPosition = targetPosition - position;
  Vector_d perp = (deltaPosition / deltaPosition.magnitude()).rotate(M_PI / 2);

  const double turretAngle = deltaPosition.angle() - rotation;

  // turn perpandicular to the target
  const double beta_error = angDiffRadians(perp.angle(), rotation);
  // turn toward the target
  const double angle_error = angDiffRadians(deltaPosition.angle(), rotation);
  // How far are we away from the target.
  const double distance_error =
      (deltaPosition.magnitude() - targetDistance) / targetDistance;

  bool shooting;
  double beta = angDiffRadians(turretAngle, r.getTurretAngle());
  if (-0.01 < beta && beta < 0.01) {
    shooting = true;
  } else {
    shooting = false;
  }

  const double w = distance_error * angle_error * K_distance +
                   (1 - distance_error) * beta_error * K_beta;
  return {r.rules.v_max, w, turretAngle, shooting};
}
