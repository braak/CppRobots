/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Wanderer.cpp
*   \author Jan-Niklas Braak
*/

#include "Agents/Wanderer.hpp"

Wanderer::Wanderer(unsigned seed, double delta_w, double v) : v(v), w(0) {
  std::default_random_engine generator(seed);
  // std::uniform_real_distribution<double> distribution(-delta_w, delta_w);
  std::normal_distribution<double> distribution(0, delta_w);

  rng = std::bind(distribution, generator);
}

Robot::Action Wanderer::update(Robot const &r) {
  w += rng();
  // prevent windup by limiting the turning rate.
  w = std::min(std::max(w, -r.w_max), r.w_max);
  return {v, w};
}
