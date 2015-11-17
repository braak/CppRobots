/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Action.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __ACTION__
#define __ACTION__

/**
\brief  Action to be perfomes during a timeStep.
*/
struct Action {
  double v; //!< The desired speed of the Robot.
  double w; //!< The desired turning rate of the Robot.
  double turretAngle;
  bool shooting;
};

#endif /* end of include guard: __ACTION__ */
