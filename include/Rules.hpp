/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Rules.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __RULES__
#define __RULES__

#include "Vector.hpp"

/**
  The rules of the game.
*/
struct Rules {
  double timeStep;          //!<
  double scan_range;        //!< Vision range of the Robot.
  double scan_angle;        //!< Field of Veiw of the Robot.
  Vector_d robot_size;      //!< size of the Robot.
  Vector_d arena_size;      //!< size of the Arena.
  double v_max;             //!< Maximal velocity of the Robot.
  double v_min;             //!< Minimal velocity of the Robot.
  double w_max;             //!< Maximal turning rate of the Robot.
  double turret_w_max;      //!< Maximal turning rate of the Robots turret.
  double collision_damage;  //!< Damage taken from collision per timeStep.
  double max_health;        //!< Maximal Health of a Robtot.
  Vector_d projectile_size; //!< size of the Robot.
  double projectile_speed;
  double projectile_cooldown;
  double projectile_damage;

  constexpr static Rules defaultRules() {
    return {1 / 10.0,
            1000.0,
            M_PI / 3,
            {30, 18},
            {1500, 1000},
            60,
            -15,
            0.6,
            3.14,
            5,
            100,
            {4, 4},
            500,
            0.8,
            20};
  }
};

#endif /* end of include guard: __RULES__ */
