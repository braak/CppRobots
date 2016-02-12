/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Rules.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __RULES__
#define __RULES__

#include "Vector.hpp"
#include "json/json.h"

#include <iostream>

/**
  The rules of the game.
*/
struct Rules {
  double timeStep;            //!< The time one Simulation takes.
  double scan_range;          //!< Vision range of the Robot.
  double scan_proximity;      //!< Vision range of the Robot.
  double scan_angle;          //!< Field of Veiw of the Robot.
  Vector_d robot_size;        //!< Size of the Robot.
  Vector_d arena_size;        //!< Size of the Arena.
  double v_max;               //!< Maximal velocity of the Robot.
  double v_min;               //!< Minimal velocity of the Robot.
  double w_max;               //!< Maximal turning rate of the Robot.
  double turret_w_max;        //!< Maximal turning rate of the Robots turret.
  double collision_damage;    //!< Damage taken from collision per timeStep.
  double max_health;          //!< Maximal Health of a Robtot.
  Vector_d projectile_size;   //!< Size of the Robot.
  double projectile_speed;    //!< Speed of a Projectile.
  double projectile_cooldown; //!< Time between shots.
  double projectile_damage;   //!< Damage a Projectile deals.

  /**
    Default Rules.
    \return Rules initialized with default values.
  */
  constexpr static Rules defaultRules() {
    return {1 / 60.0,
            1000.0,
            100,
            M_PI / 3,
            {30, 18},
            {1500, 1000},
            100,
            -30,
            0.6,
            3.14,
            5,
            100,
            {4, 4},
            1000,
            0.4,
            10};
  }
};

/**
  Output stream Operator.
  Write the Rules to the stream as a Json object.
  \param os The Output stream.
  \param rules The Rules.
  \return The Output stream.
*/
std::ostream &operator<<(std::ostream &os, const Rules &rules);
/**
  Input stream Operator.
  Read the Rules from the stream as a Json object.
  \param is The Input stream.
  \param rules The Rules.
  \return The Input stream.
*/
std::istream &operator>>(std::istream &is, Rules &rules);

#endif /* end of include guard: __RULES__ */
