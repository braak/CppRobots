/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Wanderer.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __WANDERER__
#define __WANDERER__

#include "Agent.hpp"
#include "Robot.hpp"

#include <algorithm>

/**
  \brief A  rudimentary Agent, that drives around randomly (wanders).
*/
class Wanderer final : public Agent {
  const double v;
  double w;
  std::function<double(void)> rng;

public:
  /**
    \brief Constructor.

    \param seed a seed for the internal random number generator.
    \param delta_w the maximal change in turning rate per time step.
    \param v the velocity.
  */
  Wanderer(unsigned seed, double delta_w, double v);

  /**
    During ech time step the Wanderer changes his turning rate by a random
    amount. The turning rate is capped at a maximal value. The tangential speed
    is constant.
  */
  virtual Robot::Action update(Robot const &r) override;
};

#endif /* end of include guard: __WANDERER__ */
