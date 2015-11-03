/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Agent.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __AGENT__
#define __AGENT__

#include "Robot.hpp"

class Robot;
struct Action;
/**
  \brief  Abstract Agent class.

  A Agent perfomes the descicions for a Robot. It can be seen as its 'Brain'.
  A specific Agent has to overload the update method.
*/
class Agent {
public:
  /**
  This method is called each time step, its retrun value is passed to
  Robot.update. The Agent has access to the public fields of the Robot. Using
  the available information the Agent composes a Action to be performed this
  time step.
  \param r A constant refference to the Robot.
  \return The Action to be performed this time step.
  */
  virtual Action update(Robot const &r) = 0;
};

#endif /* end of include guard: __AGENT__ */
