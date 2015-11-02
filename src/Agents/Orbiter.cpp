/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Orbiter.cpp
*   \author Jan-Niklas Braak
*/

#include "Agents/Orbiter.hpp"

Orbiter::Orbiter(double v, double w) : v(v), w(w) {}

Robot::Action Orbiter::update(Robot const &r) {
  (void)r; // surpress unused parameter warning
  return {v, w, 0};
}
