/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Player.cpp
*   \author Jan-Niklas Braak
*/

#include "Player.hpp"

Player::Player(const double &timeStep, const Vector_d &size)
    : robot(timeStep, size) {}

Player::Player(Player &&player) noexcept : robot(std::move(player.robot)),
                                           agent(std::move(player.agent)) {}

void Player::update() {
  if (!agent) {
    throw std::runtime_error("No Agent was set for this Player.");
  }
  Robot::Action action = agent->update(robot);

  robot.update(action);
}

void Player::setAgent(Agent *agent_) { agent = std::unique_ptr<Agent>(agent_); }
// void Player::setPose(Pose pose) { robot.setPose(pose); }

void Player::setPosition(Vector_d position) { robot.setPosition(position); }
Vector_d Player::getPosition() const { return robot.getPosition(); }
double Player::getRotation() const { return robot.getRotation(); }

void Player::onCollision() { robot.onCollision(); }
double Player::getHealth() const { return robot.getHealth(); }

void Player::setScanTargets(std::list<std::shared_ptr<Robot>> scanTargets) {
  robot.setScanTargets(scanTargets);
}

const Robot &Player::getRobot() const { return robot; }

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  Rectangle body = robot.getBody();

  sf::RectangleShape rect({(float)body.getSize().x, (float)body.getSize().y});
  rect.setPosition({(float)body.getPosition().x, (float)body.getPosition().y});
  rect.setRotation(degrees(body.getRotation()));
  rect.setOrigin(0.5 * body.getSize().x, 0.5 * body.getSize().y);
  double a = robot.getHealth() / 100.0;
  if (a > 0)
    rect.setFillColor({(uint8_t)(255 * (1 - a)), (uint8_t)(255 * a), 0});

  target.draw(rect, states);
}
