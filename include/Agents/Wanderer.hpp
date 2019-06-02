/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Wanderer.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __WANDERER__
#define __WANDERER__

#include "Agent.hpp"
#include "Robot.hpp"

#include "mathUtility.hpp"

#include <algorithm>
#include <random>
#include <functional>

/**
  \brief A  rudimentary Agent, that moves around randomly (wanders).
*/
class Wanderer final : public Agent {
  const double v;
  double w;
  std::function<double(void)> random;

public:
  /**
    \brief Constructor.

    \param seed Seed for a random number generator
    \param delta_w the maximal change in turning rate per time step.
    \param v the velocity.
  */

  Wanderer(double delta_w, double v, unsigned int seed);

  /**
    During ech time step the Wanderer changes his turning rate by a random
    amount. The turning rate is capped at a maximal value. The tangential speed
    is constant.
  */
  virtual Action update(Robot const &r) override;
};

#endif /* end of include guard: __WANDERER__ */
