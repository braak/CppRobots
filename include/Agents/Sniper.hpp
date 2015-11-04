/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Sniper.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __SNIPER__
#define __SNIPER__

#include "Agent.hpp"
#include "Robot.hpp"
#include "Angles.hpp"

#include <iostream>
/**
  \brief An Agent that shoots the closes visible Robot.
*/
class Sniper final : public Agent {

public:
  /**
  Construct a Sniper.
  */
  Sniper();

  /**
  During each time step the Sniper trys to get clother to the closes visible
  Robot.
  */
  virtual Action update(Robot const &r) override;
};

#endif /* end of include guard: __SNIPER__ */
