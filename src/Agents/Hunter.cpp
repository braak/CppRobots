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
  const auto target_robot = r.scanClosest();
  const auto position = r.getPosition();
  const auto rotation = r.getRotation();

  /*
    const auto target_position =
        target_robot ?  target_robot->getPosition() : r.rules.arena_size / 2.0;

    const auto delta_position = target_position - position;

    const auto turret_angle = target_robot
                                  ? delta_position.angle() - rotation :
                                  r.getTurretAngle() + r.rules.scan_angle;
  */

  double turret_angle;
  Vector_d delta_position;

  if (!target_robot) {
    // Set the center of the arena as target.
    const auto target_position = r.rules.arena_size / 2.0;
    delta_position = target_position - position;

    // Rotate the Turret to scan for targets
    turret_angle = r.getTurretAngle() + r.rules.scan_angle;
  } else {
    // Set the found Robot as Target.
    const auto target_position = target_robot->getPosition();
    delta_position = target_position - position;

    // Aim at target
    turret_angle = delta_position.angle() - rotation;
  }

  // calculate w for the two rules.
  const auto w_toward = turn_toward(delta_position.angle(), rotation);
  const auto w_perp = turn_perpendicular(delta_position.angle(), rotation);

  // How far are we away from the target distance, as a value from -1 to 1.
  const auto distance_error =
      clamp(delta_position.magnitude() / targetDistance - 1.0, -1.0, 1.0);

  /*
   * Put it all together. Linearly interpolate between driving toward the target
   * and driving perpendicular to it depending on the distance. So that at the
   * target distance we drive fully perpendicular, if we are farher away we
   * drive toward the target and if we are to close we drive away from the
   * target (due to the negative sign of the distance error).
   */
  const auto w = lerp(w_perp, w_toward, distance_error);

  // Is the target in front of the turret (within 0.01rad)? If yes then shoot.
  const auto turret_error = angDiffRadians(turret_angle, r.getTurretAngle());
  const auto shooting = fabs(turret_error) < 0.01 && target_robot;

  // drive at full speed.
  const auto v = r.rules.v_max;

  return {v, w, turret_angle, shooting};
}

double Hunter::turn_perpendicular(double angle, double rotation) const {
  // turn perpandicular to the target
  const auto perp_error1 = angDiffRadians(angle + M_PI / 2, rotation);
  const auto perp_error2 = angDiffRadians(angle - M_PI / 2, rotation);
  const auto perp_error =
      fabs(perp_error1) < fabs(perp_error2) ? perp_error1 : perp_error2;
  const auto w = perp_error * K_perp;

  return w;
}

double Hunter::turn_toward(double angle, double rotation) const {
  const auto straight_error = angDiffRadians(angle, rotation);
  const auto w = straight_error * K_straight;

  return w;
}
