/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Follower.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __FOLLOWER__
#define __FOLLOWER__

#include "Agent.hpp"
#include "Robot.hpp"
#include "Angles.hpp"

#include <iostream>
/**
  \brief An Agent that follows the closes visible Robot.
*/
class Follower final : public Agent {
private:
  double target_distance;
  double K_distance;
  double K_angle;

public:
  /**
  Construct a Follower with the given Controll parameters.
  \param target_distance how close the Follower trys to get to its target.
  \param K_distance gain of the distance controller.
  \param K_angle gain of the angle controller.
  */
  Follower(double target_distance, double K_distance, double K_angle);

  /**
  During each time step the Follower trys to get clother to the closes visible
  Robot.
  */
  virtual Action update(Robot const &r) override;
};

#endif /* end of include guard: __FOLLOWER__ */
