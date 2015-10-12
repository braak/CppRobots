/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Player.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __PLAYER__
#define __PLAYER__

#include <memory>
#include <exception>
#include <SFML/Graphics.hpp>
#include "Pose.hpp"
#include "Robot.hpp"
#include "Agent.hpp"
#include "Angles.hpp"

/**
  \brief Encapsulates the behavior of a player.

  A Player has an Agent an a Robot object. during each time step the
  Agent makes a descision, that descision is passed to the Robot, which performs
  the aprpriate actions. The Player inherrits from sf::Drawable and can be drawn
  to a RenderTarget.
*/
class Player : public sf::Drawable {
private:
  Robot robot;
  std::unique_ptr<Agent> agent;
  sf::RectangleShape rectangle;

public:
  /**
    Constructor.

    \param timeStep the duration between each time step.
    \param size the size of the Rectangle representing the Player
  */
  Player(const double &timeStep, const sf::Vector2f &size);

  /**
   Move Constructor.
   \param player the Player to move.
  */
  Player(Player &&player) noexcept;

  /**
    Perfom one time step.

    Updates the Agent and the Robot.
    \exception Throws an exception when no Agent is set.
    \see Agent::update, Robot::update
  */
  void update();

  /**
    set the current Agent.
    \param agent An Agent.
  */
  void setAgent(Agent *agent);
  /**
    set the Pose of the Robot.
    \param pose the new Pose
  */
  void setPose(Pose pose);
  /**
    get the current Pose of the Robot.
    \return the current Pose of the Robot
  */
  Pose getPose() const;

  void setScanTargets(std::list<std::shared_ptr<Robot>> scanTargets);
  const Robot &getRobot() const;

private:
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const override;
};

#endif /* end of include guard: __PLAYER__ */
