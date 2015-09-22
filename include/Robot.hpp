/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Robot.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __ROBOT__
#define __ROBOT__

#include <math.h>
#include <string>

#include "Pose.hpp"

/**
    \brief The Robot class represents the movement behavior, position and
   orientation of a player.

   The movement of a Robot works like that of a tank. The Robot can
   drive forward and turn but it can not move sideward.
*/
class Robot {
private:
  Pose pose;
  // double x;
  // double y;
  // double theta;

  const double timeStep;
  friend std::ostream& operator<<(std::ostream& os, const Robot& obj);
public:
  /**
  constructs a Robot with a specified timeStep. The timeStep determins the
  simulation speed.
  */
  Robot(const double timeStep = 1);

  virtual ~Robot();

  /**
      updates the position and orientation of the Robot acording to the two
     input values.
      \param v The velocity of the Robot.
      \param w The angular velocity(turning rate) of the Robot.
  */
  void update(double v, double w);

  /**
  Getter for the Pose of the Robot.
  \return the Pose of the Robot.
  */
  Pose getPose() const;
  /**
  Setter for the Pose of the Robot.
  \param pose The new Pose of the Robot.
  */
  void setPose(Pose pose);

};

/**
  Output stream Operator.
*/
std::ostream& operator<<(std::ostream& os, const Robot& obj);

#endif /* end of include guard: __ROBOT__ */
