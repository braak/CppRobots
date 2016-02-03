/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Hunter.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __HUNTER__
#define __HUNTER__

#include "Agent.hpp"
#include "Robot.hpp"
#include "Angles.hpp"

#include "mathUtility.hpp"

#include <iostream>
/**
  \brief An Agent that shoots the closes visible Robot.
*/
class Hunter final : public Agent {
  double targetDistance;
  double K_beta;
  double K_distance;

public:
  /**
  Construct a Hunter.
  */
  Hunter(double targetDistanc, double K_beta, double K_distance);

  /**
  During each time step the Hunter trys to get clother to the closes visible
  Robot.
  */
  virtual Action update(Robot const &r) override;
};

#endif /* end of include guard: __HUNTER__ */
