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
#include "Rectangle.hpp"

/**
    \brief The Robot class represents the movement behavior, position
   and orientation of a player.
*/
class Robot {
  // Data and declarations
public:
  const double v_max = 60;  //!< Maximal velocity of the Robot
  const double v_min = -8;  //!< Minimal velocity of the Robot
  const double w_max = 0.6; //!< Maximal turning rate of the Robot.

protected:
  Rectangle body; //!< The body of the Robot. Represents its location, heading
  // and dimentions.

private:
  double timeStep;
  std::list<std::shared_ptr<Robot>> scanTargets;
  double health = 100.0;

  // Methodes
public:
  /**
  \brief  Action to be perfomes during a timeStep.
  */
  struct Action {
    double v; //!< The desired speed of the Robot.
    double w; //!< The desired turning rate of the Robot.
  };
  /**
  constructs a Robot with a specified timeStep. The timeStep determins the
  simulation speed.
  */
  Robot(const double timeStep, Vector_d size);

  /**
      updates the position and orientation of the Robot acording to the
     Action.
      \param a An Action object
  */
  virtual void update(Action const &a);

  /**
  Setter for the Pose of the Robot.
  \param pose The new Pose of the Robot.
  */
  void setPose(Pose pose);

  /**
  Get method for the position of the Robot.
  \return the position of the Robot.
  */
  Vector_d getPosition() const;
  void setPosition(Vector_d position);

  /**
  Get method for the rotation of the Robot.
  \return the rotation of the Robot.
  */
  double getRotation() const;

  /**
  Get method for the Robot body.
  \return the body of the Robot.
  */
  const Rectangle &getBody() const;
  /**
  set the list of visible Robots.
  \param scanTargets the list of visible Robots.
  */
  void setScanTargets(std::list<std::shared_ptr<Robot>> scanTargets);
  /**
  get a list of all visible Robots.
  \return the list of visible Robots.
  */
  std::list<std::shared_ptr<Robot>> getScanTargets() const;

  void onCollision();

  double getHealth() const;

private:
  friend std::ostream &operator<<(std::ostream &os, const Robot &obj);
};

/**
  Output stream Operator.
*/
std::ostream &operator<<(std::ostream &os, const Robot &obj);

#endif /* end of include guard: __ROBOT__ */
