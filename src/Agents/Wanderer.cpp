/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Wanderer.cpp
*   \author Jan-Niklas Braak
*/

#include "Agents/Wanderer.hpp"

Wanderer::Wanderer(unsigned seed, double max_w, double delta_w, double v)
    : v(v), max_w(max_w), w(0) {
  std::default_random_engine generator(seed);
  // std::uniform_real_distribution<double> distribution(-delta_w, delta_w);
  std::normal_distribution<double> distribution(0, delta_w);

  rng = std::bind(distribution, generator);
}

Robot::Action Wanderer::update(Robot const &r) {
  (void)(r);
  w += rng();
  w = std::min(std::max(w, -max_w), max_w);
  return {v, w};
}
