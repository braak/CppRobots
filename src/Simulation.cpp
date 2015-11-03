/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Simulation.cpp
*   \author Jan-Niklas Braak
*/

#include "Simulation.hpp"

Simulation::Simulation(sf::Font &font, std::default_random_engine seed,
                       const Rules &rules)
    : rules(rules), font(font), generator(seed) {}

void Simulation::updatePlayers() {
  // set vision for all players
  for (auto &player : players) {
    check_scan(player.second);
  }
  // move the player and spawn projectiles
  for (auto &player : players) {
    player.second.update();
  }

  // check collision between playeres
  // std::list<Robot *> collisions;
  // NOTE: currently we check each pair of players twice, once for
  // Collision(A,B) and once for Collision(B,A).
  for (auto &player1 : players) {
    for (auto const &player2 : players) {
      if (&player1 == &player2) {
        // dont check collision with self.
        continue;
      }
      // Collision collision(player1.second, player2.second);
      if (Collision(player1.second, player2.second)) {
        player1.second.takeDamage(rules.collision_damage);
      }
    }
  }

  // check if any player is outside the arena
  for (auto &player : players) {
    Vector_d pos = player.second.getPosition();
    if (pos.x > rules.arena_size.x || pos.y > rules.arena_size.y || pos.x < 0 ||
        pos.y < 0) {
      player.second.takeDamage(rules.collision_damage);
    }
  }

  // see if any player wants to shoot
  for (auto &player : players) {
    if (player.second.shooting) {
      player.second.shooting = false;
      double direction =
          player.second.getRotation() + player.second.getTurretAngle();
      Vector_d porjectilePosition = player.second.getPosition();
      // make sure we create the Projectile outside the player
      porjectilePosition += Vector_d::polar(direction, rules.robot_size.x + 1);
      projectiles.push_back(Projectile(rules, porjectilePosition, direction));
    }
  }
}

void Simulation::updateProjectiles() {
  // move the projectile
  for (auto &projectle : projectiles) {
    projectle.update();
  }
  // remove projectile if it is out of bounds
  projectiles.remove_if([this](const Projectile &projectle) {
    Vector_d pos = projectle.getBody().getPosition();
    return pos.x > rules.arena_size.x || pos.y > rules.arena_size.y ||
           pos.x < 0 || pos.y < 0;
  });

  // check collision between player and projectile
  for (auto &player : players) {
    for (auto projectile = projectiles.begin();
         projectile != projectiles.end();) {
      Collision collision(player.second, *projectile);
      if (collision) {
        // remove projectile, and advance the iterator
        projectile = projectiles.erase(projectile);
        // deal damage to the player
        player.second.takeDamage(rules.projectile_damage);
      } else {
        // advance the iterator
        ++projectile;
      }
    }
  }
}
void Simulation::update() {
  updatePlayers();
  updateProjectiles();

  // remove players that don't have health left.
  auto pred = [](const std::pair<const std::string, Robot> &robot) {
    return robot.second.getHealth() <= 0;
  };
  auto it = players.begin();
  while ((it = std::find_if(it, players.end(), pred)) != players.end()) {
    players.erase(it++);
  }
}

void Simulation::addPlayer(std::string name, Robot &player) {
  players.insert(KeyValuePair(name, std::move(player)));
}
void Simulation::newPlayer(std::string name, Agent *agent) {
  Robot player(rules, agent);

  std::uniform_real_distribution<double> pos_x(0, rules.arena_size.x);
  std::uniform_real_distribution<double> pos_y(0, rules.arena_size.y);
  std::uniform_real_distribution<double> rot(0, 2 * M_PI);
  player.setPosition({pos_x(generator), pos_y(generator)});
  player.setRotation(rot(generator));

  addPlayer(name, player);
}
bool Simulation::inSector(Vector_d const &p1, double rotation,
                          Vector_d const &p2) const {
  const Vector_d v = p2 - p1;

  const bool in_range = v.magnitude() < rules.scan_range;

  const double beta = angDiffRadians(rotation, v.angle());
  const bool in_segment =
      -rules.scan_angle / 2 < beta && beta < rules.scan_angle / 2;

  return in_range && in_segment;
}

void Simulation::check_scan(Robot &robot) {
  std::list<std::shared_ptr<Robot>> scanTargets;
  for (auto const &player2 : players) {
    if (&robot == &player2.second) {
      continue;
    }
    Vector_d pose1 = robot.getPosition();
    double rotation = robot.getRotation() + robot.getTurretAngle();
    Vector_d pose2 = player2.second.getPosition();
    if (inSector(pose1, rotation, pose2)) {
      scanTargets.push_back(std::make_shared<Robot>(player2.second));
    }
  }
  robot.setScanTargets(std::move(scanTargets));
}

void Simulation::drawProjectile(sf::RenderTarget &target,
                                sf::RenderStates states,
                                const Projectile &projectile) const {
  Rectangle body = projectile.getBody();

  sf::RectangleShape rect({(float)body.getSize().x, (float)body.getSize().y});
  rect.setOrigin(0.5 * body.getSize().x, 0.5 * body.getSize().y);
  rect.setPosition({(float)body.getPosition().x, (float)body.getPosition().y});
  rect.setRotation(degrees(body.getRotation()));
  target.draw(rect, states);
}
void Simulation::drawRobot(sf::RenderTarget &target, sf::RenderStates states,
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

void Simulation::drawArc(sf::RenderTarget &target, sf::RenderStates states,
                         Vector_d position, double rotation, double radius,
                         double angle) const {
  /* NOTE: this function is very inefficient. It can be improved by having a
  constant Arc object and modifiing its position via the transformation
  matrix of RenderStates.
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

void Simulation::drawPlayer(sf::RenderTarget &target, sf::RenderStates states,
                            const std::string &name, const Robot &robot) const {
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

void Simulation::draw(sf::RenderTarget &target, sf::RenderStates states) const {
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
