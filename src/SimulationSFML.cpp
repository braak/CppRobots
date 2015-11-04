/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file SimulationSFML.cpp
*   \author Jan-Niklas Braak
*/

#include "SimulationSFML.hpp"

SimulationSFML::SimulationSFML(const Rules &rules,
                               std::default_random_engine rng, sf::Font font)
    : Simulation(rules, rng), font(font) {}

void SimulationSFML::drawProjectile(sf::RenderTarget &target,
                                    sf::RenderStates states,
                                    const Projectile &projectile) const {
  Rectangle body = projectile.getBody();

  sf::RectangleShape rect({(float)body.getSize().x, (float)body.getSize().y});
  rect.setOrigin(0.5 * body.getSize().x, 0.5 * body.getSize().y);
  rect.setPosition({(float)body.getPosition().x, (float)body.getPosition().y});
  rect.setRotation(degrees(body.getRotation()));
  target.draw(rect, states);
}
void SimulationSFML::drawRobot(sf::RenderTarget &target,
                               sf::RenderStates states,
                               const Robot &robot) const {
  Rectangle body = robot.getBody();

  sf::RectangleShape rect({(float)body.getSize().x, (float)body.getSize().y});
  rect.setOrigin(0.5 * body.getSize().x, 0.5 * body.getSize().y);
  rect.setPosition({(float)body.getPosition().x, (float)body.getPosition().y});
  rect.setRotation(degrees(body.getRotation()));
  double a = robot.getHealth() / 100.0;
  if (a > 0)
    rect.setFillColor({(uint8_t)(255 * (1 - a)), (uint8_t)(255 * a), 0});
  target.draw(rect, states);

  sf::RectangleShape turret(
      {(float)body.getSize().x, (float)body.getSize().y / 3});
  turret.setPosition(
      {(float)body.getPosition().x, (float)body.getPosition().y});
  turret.setRotation(degrees(body.getRotation() + robot.getTurretAngle()));
  turret.setOrigin(0.5 * body.getSize().x / 4, 0.5 * body.getSize().x / 4);

  target.draw(turret, states);
}

void SimulationSFML::drawArc(sf::RenderTarget &target, sf::RenderStates states,
                             Vector_d position, double rotation, double radius,
                             double angle) const {
  /* NOTE: this function is very inefficient. It can be improved by having a
  constant Arc object and modifiing its position via the transformation
  matrix of RenderStates acording to the player position.
  */
  sf::VertexArray lines(sf::TrianglesFan);
  lines.append({{(float)position.x, (float)position.y}, {0, 0, 255, 60}});

  for (double alpha = -angle / 2; alpha <= angle / 2; alpha += angle / 16) {
    float x = position.x + radius * cos(rotation - alpha);
    float y = position.y + radius * sin(rotation - alpha);
    lines.append({{x, y}, {0, 0, 255, 60}});
  }
  target.draw(lines, states);
}

void SimulationSFML::drawPlayer(sf::RenderTarget &target,
                                sf::RenderStates states,
                                const std::string &name,
                                const Robot &robot) const {
  // drawRobot
  drawRobot(target, states, robot);

  // drawLable
  Vector_d p = robot.getPosition();

  sf::Text name_tag(name, font, 15);
  name_tag.setOrigin(0.5 * name_tag.getLocalBounds().width, 0);
  name_tag.setPosition({(float)p.x, (float)p.y + 15});

  target.draw(name_tag, states);

  // drawArc(target, states, p, robot.getRotation() + robot.getTurretAngle(),
  //         rules.scan_range, rules.scan_angle);
}

void SimulationSFML::draw(sf::RenderTarget &target,
                          sf::RenderStates states) const {
  /*NOTE: This can be significantly simplified by using drawPlayer and calling
     drawRobot, drawLable and drawArc after modifying states.transform
  */
  sf::RectangleShape rect(
      {(float)rules.arena_size.x, (float)rules.arena_size.y});
  rect.setFillColor(sf::Color::Transparent);
  rect.setOutlineColor(sf::Color::White);
  rect.setOutlineThickness(10);
  target.draw(rect, states);

  for (auto const &player : players) {
    drawPlayer(target, states, player.first, player.second);
  }

  for (auto const &projectile : projectiles) {
    drawProjectile(target, states, projectile);
  }
}
