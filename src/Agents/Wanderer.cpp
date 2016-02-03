/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Wanderer.cpp
*   \author Jan-Niklas Braak
*/

#include "Agents/Wanderer.hpp"

Wanderer::Wanderer(double delta_w, double v, unsigned int seed) : v(v), w(0) {
  std::default_random_engine gen(seed);
  std::normal_distribution<double> distribution(0, delta_w);
  random = std::bind(distribution, gen);
}

Action Wanderer::update(Robot const &r) {
  // integrate a random number
  w += random();
  // prevent windup by limiting the turning rate to the maximal output.
  w = clamp(w, -r.rules.w_max, r.rules.w_max);
  return {v, w, 0, false};
}
