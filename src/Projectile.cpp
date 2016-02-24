/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Projectile.cpp
*   \author Jan-Niklas Braak
*/

#include "Projectile.hpp"
Projectile::Projectile(const Rules &rules, Vector_d position, double direction,
                       std::string owner)
    : rules(rules), body(rules.projectile_size, position, direction),
      owner(owner) {}

void Projectile::update() {
  // Move forward
  body.move(Vector_d::polar(body.getRotation(),
                            rules.projectile_speed * rules.timeStep));
}

const Rectangle &Projectile::getBody() const { return body; }
