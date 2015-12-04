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
  auto target_robot = r.scanClosest();

  Vector_d position = r.getPosition();
  double rotation = r.getRotation();

  if (!target_robot) {
    return {0, 0, r.getTurretAngle() + r.rules.scan_angle, false};
  }

  Vector_d targetPosition = target_robot->getPosition();
  Vector_d deltaPosition = targetPosition - position;

  double angle = wrapRadians(deltaPosition.angle() - rotation);

  bool shooting;
  double beta = angDiffRadians(angle, r.getTurretAngle());
  if (-0.01 < beta && beta < 0.01) {
    shooting = true;
  } else {
    shooting = false;
  }

  return {0, 0, angle, shooting};
}
