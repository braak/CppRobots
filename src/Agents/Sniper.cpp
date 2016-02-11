/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Sniper.cpp
*   \author Jan-Niklas Braak
*/

#include <cmath>

#include "Agents/Sniper.hpp"
#include "Angles.hpp"
#include "math.h"

Sniper::Sniper() {}

Action Sniper::update(Robot const &r) {
  const auto target_robot = r.scanClosest();

  if (!target_robot) {
    const auto turretAngle =
        wrapRadians(r.getTurretAngle() + r.rules.scan_angle);
    return {0, 0, turretAngle, false};
  }

  const auto position = r.getPosition();
  const auto rotation = r.getRotation();

  const auto targetPosition = target_robot->getPosition();
  const auto deltaPosition = targetPosition - position;

  const auto turretAngle = wrapRadians(deltaPosition.angle() - rotation);
  const auto turret_error = angDiffRadians(turretAngle, r.getTurretAngle());
  const auto shooting = fabs(turret_error) < 0.01;

  return {0, 0, turretAngle, shooting};
}
