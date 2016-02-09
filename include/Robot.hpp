/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
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

#include "Rectangle.hpp"
#include "Rules.hpp"
#include "Action.hpp"
#include "Agent.hpp"
#include "mathUtility.hpp"

class Agent;

/**
    \brief The Robot class represents the movement behavior, position
   and orientation of a player.
*/
class Robot {
  // Data and declarations
public:
  /**
    The Rules used in the Simulation.
  */
  const Rules &rules;

private:
  Rectangle body;
  double health;

  std::list<std::shared_ptr<Robot>> scanTargets;
  double turretAngle;
  // double v;
  std::shared_ptr<Agent> agent;
  double cooldown;

  bool shooting;

  // Methodes
public:
  /**
  Constructs a Robot with a specified timeStep. The timeStep determins the
  simulation speed.
  \param rules the Rules of the game.
  \param agent the Agent ust to controll the Robot.
  */
  Robot(const Rules &rules, Agent *agent);

  // Robot(const Rules &rules, Agent *agent, Vector_d position, double
  // rotation);

  /**
  Get method for the position of the Robot.
  \return the position of the Robot.
  */
  Vector_d getPosition() const;

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
    Get a list of all visible Robots.
    \return the list of visible Robots.
  */
  const std::list<std::shared_ptr<Robot>> scanAll() const;

  /**
    Get the closest visible Robot.
    \return the closest visible Robot.
  */
  std::shared_ptr<const Robot> scanClosest() const;
  /**
    Get one visible Robot.
    \return One visible Robot.
  */
  std::shared_ptr<const Robot> scanAny() const;

  /**
  Get the current health of the Robot.
  \return the current health of the Robot.
  */
  double getHealth() const;

  /**
  Get the angle of the turret relative to the body.

  \return the angle of the turret relative to the body.
  */
  double getTurretAngle() const;

private:
  friend class Simulation;

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
  Set method for the Position.
  \param position the new position.
  */
  void setPosition(Vector_d position);

  /**
  Set method for the Rotation.
  \param rotation the new rotation.
  */
  void setRotation(double rotation);

  /**
  set the list of visible Robots.
  \param scanTargets the list of visible Robots.
  */
  void setScanTargets(std::list<std::shared_ptr<Robot>> scanTargets);

  /**
  Method that is called each time there is a collision.
  */
  // NOTE: we could pass the collision target so we can do better collision
  // resolution
  void onCollision();

  double takeDamage(double damage);

  friend std::ostream &operator<<(std::ostream &os, const Robot &obj);
};

/**
  Output stream Operator.
*/
std::ostream &operator<<(std::ostream &os, const Robot &obj);

#endif /* end of include guard: __ROBOT__ */
