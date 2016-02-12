/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
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
  double timeStep = 1 / 60.0;         //!< The time one Simulation takes.
  double scan_range = 1000;           //!< Vision range of the Robot.
  double scan_proximity = 100;        //!< Vision range of the Robot.
  double scan_angle = M_PI / 3;       //!< Field of Veiw of the Robot.
  Vector_d robot_size = {30, 18};     //!< Size of the Robot.
  Vector_d arena_size = {1500, 1000}; //!< Size of the Arena.
  double v_max = 100;                 //!< Maximal velocity of the Robot.
  double v_min = -30;                 //!< Minimal velocity of the Robot.
  double w_max = 0.6;                 //!< Maximal turning rate of the Robot.
  double turret_w_max = 3.14;  //!< Maximal turning rate of the Robots turret.
  double collision_damage = 5; //!< Damage taken from collision per timeStep.
  double max_health = 100;     //!< Maximal Health of a Robtot.
  Vector_d projectile_size = {4, 4}; //!< Size of the Robot.
  double projectile_speed = 1000;    //!< Speed of a Projectile.
  double projectile_cooldown = 0.4;  //!< Time between shots.
  double projectile_damage = 10;     //!< Damage a Projectile deals.
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
