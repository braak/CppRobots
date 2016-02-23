/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
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
  double K_perp;
  double K_straight;

public:
  /**
  Construct a Hunter.
  */
  Hunter(double targetDistanc, double K_perp, double K_straight);

  /**
  During each time step the Hunter trys to get clother to the closes visible
  Robot.
  */
  virtual Action update(Robot const &r) override;

private:
  double turn_perpendicular(double ang, double rotation) const;
  double turn_toward(double ang, double rotation) const;
};

#endif /* end of include guard: __HUNTER__ */
