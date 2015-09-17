/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Robot.hpp
*   \author Jan-Niklas Braak
*/

#include <math.h>
#include <string>

struct Pose {
  double x;
  double y;
  double theta;
  Pose(double x, double y, double theta) : x(x), y(y), theta(theta) {}
};

/**
    \brief The Robot class represents the movement behavior, position and
   orientation of a player.

   The movement of a Robot works like that of a tank. The Robot can
   drive forward and turn but it can not move sideward.
*/
class Robot {
private:
  double x;
  double y;
  double theta;

  const double timeBase;

public:
  /**
  constructs a Robot with a specified timeBase. The timeBase determins the
  simulation speed.
  */
  Robot(const double timeBase = 1);

  virtual ~Robot();

  /**
      updates the position and orientation of the Robot acording to the two
     input values.
      \param v The velocity of the Robot.
      \param w The angular velocity(turning rate) of the Robot.
  */
  void update(double v, double w);

  Pose getPose();
  void setPose(Pose p);
  /**
      Method for conveniently printing the state of the Robot.
      \return a string in the form "<Robot at x, y, theta>", where x and y are
     the current position of the Robot and theta the current orientation.
  */
  operator std::string();
};
