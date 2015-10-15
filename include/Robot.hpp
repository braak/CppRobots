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
#include <list>
#include <memory>
#include <algorithm>

#include "Pose.hpp"

/**
    \brief The Robot class represents the movement behavior, position
   and orientation of a player.
*/
class Robot {
  // Data and declarations
public:
  const double v_max = 60;
  const double v_min = -8;
  const double w_max = 0.6;

protected:
  Pose pose; //!< The Location and Oriantation of the Robot.

private:
  double timeStep;
  std::list<std::shared_ptr<Robot>> scanTargets;

  // Methodes
public:
  /**
  \brief  Action to be perfomes during a timeStep.
  */
  struct Action {
    double v; //!< The speed of the Robot.
    double w; //!< The turning rate of the Robot.
  };
  /**
  constructs a Robot with a specified timeStep. The timeStep determins the
  simulation speed.
  */
  Robot(const double timeStep = 1);

  /**
      updates the position and orientation of the Robot acording to the
     Action.
      \param a An Action object
  */
  virtual void update(Action const &a);

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

  /**
  set the list of visible Robots.
  \param scanTargets the list of visible Robots.
  */
  void setScanTargets(std::list<std::shared_ptr<Robot>> scanTargets);
  /**
  get a list of all visible Robots.
  */
  std::list<std::shared_ptr<Robot>> getScanTargets() const;

private:
  friend std::ostream &operator<<(std::ostream &os, const Robot &obj);
};

/**
  Output stream Operator.
*/
std::ostream &operator<<(std::ostream &os, const Robot &obj);

#endif /* end of include guard: __ROBOT__ */
