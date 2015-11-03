/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Sniper.cpp
*   \author Jan-Niklas Braak
*/

#include "Agents/Sniper.hpp"
#include "Angles.hpp"
#include "math.h"

Sniper::Sniper() {}

Action Sniper::update(Robot const &r) {
  // return {0, 0, r.getTurretAngle() - r.rules.scan_angle, false};

  auto scanTargets = r.getScanTargets();
  Vector_d position = r.getPosition();
  double rotation = r.getRotation();

  if (scanTargets.empty()) {
    return {0, 0, r.getTurretAngle() + r.rules.scan_angle, false};
  }
  /*
    find the closes visible Robot
  */
  // NOTE: we could find the target with the smallest angles to
  // our turret, so we can shoot faster.
  auto cmp = [position](const std::shared_ptr<Robot> &a,
                        const std::shared_ptr<Robot> &b) -> bool {
    const Vector_d vector_a = a->getPosition() - position;
    const Vector_d vector_b = b->getPosition() - position;
    return vector_a.magnitude() < vector_b.magnitude();
  };
  scanTargets.sort(cmp);
  const Robot &target_robot = *scanTargets.front();

  Vector_d targetPosition = target_robot.getPosition();
  Vector_d deltaPosition = targetPosition - position;

  double angle = wrapRadians(deltaPosition.angle() - rotation);

  bool shooting;
  double beta = angDiffRadians(angle, r.getTurretAngle());
  if (-0.01 < beta && beta < 0.01) {
    shooting = true;
  } else {
    shooting = false;
  }
  // if (abs(beta) < 0.01) {
  //   shooting = true;
  // } else {
  //   shooting = false;
  // }

  return {0, 0, angle, shooting};
}
