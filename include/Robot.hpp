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
#include "Rules.hpp"
#include "Agent.hpp"

class Agent;
/**
\brief  Action to be perfomes during a timeStep.
*/
struct Action {
  double v; //!< The desired speed of the Robot.
  double w; //!< The desired turning rate of the Robot.
  double turretAngle;
};

/**
    \brief The Robot class represents the movement behavior, position
   and orientation of a player.
*/
class Robot {
  // Data and declarations
public:
  const Rules &rules;

protected:
  Rectangle body; //!< The body of the Robot. Represents its location, heading
  // and dimentions.
  double health;

private:
  // double timeStep;
  std::list<std::shared_ptr<Robot>> scanTargets;
  double turretAngle;
  // double v;
  std::shared_ptr<Agent> agent;

  // Methodes
public:
  /**
  constructs a Robot with a specified timeStep. The timeStep determins the
  simulation speed.
  \param timeStep the timeStep
  \param rules the Rules of the game.
  */
  Robot(const Rules &rules);

  // Robot(const Robot &robot);

  // /**
  // updates the position and orientation of the Robot acording to the Action.
  // \param a An Action object
  // */
  // virtual void update(Action const &a);

  /**
  updates the position and orientation of the Robot
  */
  virtual void update();

  /**
    set the current Agent.
    \param agent An Agent.
  */
  void setAgent(Agent *agent);

  /**
  Get method for the position of the Robot.
  \return the position of the Robot.
  */
  Vector_d getPosition() const;
  /**
  Set method for the Position.
  \param position the new position.
  */
  void setPosition(Vector_d position);

  /**
  Get method for the rotation of the Robot.
  \return the rotation of the Robot.
  */
  double getRotation() const;

  /**
  Set method for the Rotation.
  \param rotation the new rotation.
  */
  void setRotation(double rotation);
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

  /**
  Method that is called each time ther is a collision.
  */
  // NOTE: we should pass the collision target so we can do better collision
  // resulution
  void onCollision();

  /**
  returns the current health of the Robot.
  */
  double getHealth() const;

  double getTurretAngle() const;

private:
  double limitRate(double oldVal, double newVal, double maxRate,
                   double minRate);
  friend std::ostream &operator<<(std::ostream &os, const Robot &obj);

  Robot();
};

/**
  Output stream Operator.
*/
std::ostream &operator<<(std::ostream &os, const Robot &obj);

#endif /* end of include guard: __ROBOT__ */
