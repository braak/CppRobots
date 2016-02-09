/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Projectile.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __PROJECTILE__
#define __PROJECTILE__

#include "Rules.hpp"
#include "Rectangle.hpp"
#include "Vector.hpp"

/**
  A Projectile that moves forward at a constant speed.
*/
class Projectile {
  const Rules &rules;
  Rectangle body;

public:
  /**
    Constructor.

    \param rules The Rules of the simulation.
    \param position initial position of the Projectile.
    \param direction Directon in which the Projectile moves.
    \param owner name of the owner of the Projetile.
  */
  Projectile(const Rules &rules, Vector_d position, double direction,
             std::string owner);
  /**
    Update the position of the Projectile.
  */
  void update();

  /**
    Get the body of the Projectile.
    \return the body of the Projectile.
  */
  const Rectangle &getBody() const;

  /**
    name of the owner of the Projetile.
  */
  const std::string owner;
};

#endif /* end of include guard: __PROJECTILE__ */
