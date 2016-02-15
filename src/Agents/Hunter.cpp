/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Hunter.cpp
*   \author Jan-Niklas Braak
*/
#include <cmath>
#include <iostream>

#include "Agents/Hunter.hpp"
#include "Angles.hpp"
#include "mathUtility.hpp"

Hunter::Hunter(double targetDistance, double K_perp, double K_straight)
    : targetDistance(targetDistance), K_perp(K_perp), K_straight(K_straight) {}

Action Hunter::update(Robot const &r) {

  const auto targetRobot = r.scanClosest();
  const auto position = r.getPosition();
  const auto rotation = r.getRotation();

  // TODO: seperate movement and shooting code. Add Wall avoidance.
  double turretAngle;
  Vector_d deltaPosition;

  if (!targetRobot) {
    // Set the center of the arena as target.
    const auto targetPosition = r.rules.arena_size / 2.0;
    deltaPosition = targetPosition - position;

    // Rotate the Turret to scan for targets
    turretAngle = r.getTurretAngle() + r.rules.scan_angle;
  } else {
    // Set the found Robot as Target.
    const auto targetPosition = targetRobot->getPosition();
    deltaPosition = targetPosition - position;

    // Aim at target
    turretAngle = deltaPosition.angle() - rotation;
  }

  /*  // if a target exists, it is the target position, otherwise go to the
    center
    const auto targetPosition =
        targetRobot ? targetRobot->getPosition() : r.rules.arena_size / 2.0;
    const auto deltaPosition = targetPosition - position;

    // if a target exists turn the turret toward it, otherwise rotate
    const auto turretAngle = targetRobot
                                 ? deltaPosition.angle() - rotation
                                 : r.getTurretAngle() + r.rules.scan_angle;
*/

  // turn perpandicular to the target
  const auto perp = Vector_d(deltaPosition).rotate(M_PI / 2);
  const auto perp_error = angDiffRadians(perp.angle(), rotation);

  // turn toward the target
  const auto straight_error = angDiffRadians(deltaPosition.angle(), rotation);

  // How far are we away from the target distance, as a value from -1 to 1.
  // const auto distance_error =
  //     clamp(1.0 - targetDistance / deltaPosition.magnitude(), -1.0, 1.0);

  const auto distance_error =
      clamp((deltaPosition.magnitude() / targetDistance - 1.0), -1.0, 1.0);

  // Put it all together. Linearly interpolate between driving toward the
  // target
  // and driving perpendicular to it depending on the distance. So that at
  // the
  // target distance we drive fully perpendicular, if we are farher away we
  // drive toward the target and if we are to close we drive away from the
  // tarrget (due to the negatice sign of the distance error)
  const auto w_perp = perp_error * K_perp;
  const auto w_straight = straight_error * K_straight;
  const auto w = lerp(w_perp, w_straight, distance_error);

  // Is the target in front of the turret (within 0.01rad)? If yes then shoot.
  const auto turret_error = angDiffRadians(turretAngle, r.getTurretAngle());
  const auto shooting = fabs(turret_error) < 0.01 && targetRobot;

  // drive at full speed.
  const auto v = r.rules.v_max;

  return {v, w, turretAngle, shooting};
}
