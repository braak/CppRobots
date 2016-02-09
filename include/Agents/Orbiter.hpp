/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Orbiter.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __ORBITER__
#define __ORBITER__

#include "Agent.hpp"
#include "Robot.hpp"

/**
  \brief A very rudimentary Agent, that drives in a circle(orbit).
*/
class Orbiter final : public Agent {
  const double v;
  const double w;

public:
  /**
  The Orbiter takes a speed and a turning rate. The Orbiter will use those
  parameters to drive the Robot.
  \param v Speed.
  \param w Turning rate.
  */
  Orbiter(double v, double w);
  /**
  During each time step the Orbiter retuns its constant speed and turning rate.
  */
  virtual Action update(Robot const &r) override;
};

#endif /* end of include guard: __ORBITER__ */
