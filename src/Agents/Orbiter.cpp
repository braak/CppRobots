/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Orbiter.cpp
*   \author Jan-Niklas Braak
*/

#include "Agents/Orbiter.hpp"

Orbiter::Orbiter(double v, double w) : v(v), w(w) {}

Action Orbiter::update(Robot const &r) {
  (void)r; // suppress unused parameter warning
  return {v, w, 0, false};
}
