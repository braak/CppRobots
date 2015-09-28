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

class Player : public sf::Drawable {
private:
  Robot robot;
  std::unique_ptr<Agent> agent;
  sf::RectangleShape rectangle;

public:
  Player(const double &timeStep, const sf::Vector2f &size);

  // Move Constructor
  Player(Player &&player);

  void update();

  void setAgent(Agent *agent);
  void setPose(Pose pose);
  Pose getPose() const;

private:
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const override;
};

#endif /* end of include guard: __PLAYER__ */
