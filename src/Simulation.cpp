/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Simulation.cpp
*   \author Jan-Niklas Braak
*/

#include "Simulation.hpp"

Simulation::Simulation(sf::Font &font, std::default_random_engine seed,
                       double timeStep_)
    : rules(Rules::defaultRules()), font(font), timeStep(timeStep_),
      generator(seed) {}

void Simulation::update() {
  // set vision for all players
  for (auto &player : players) {
    check_scan(player.second);
  }

  // update all players
  for (auto &player : players) {
    player.second.update();
  }

  // check collision for all playeres
  std::list<std::string> collisions;
  // NOTE: currently we check each pair of players twice, once for each
  // direction.
  for (auto const &player1 : players) {
    for (auto const &player2 : players) {
      if (&player1 == &player2) {
        // dont check collision with self.
        continue;
      }
      Collision collision(player1.second.getRobot().getBody(),
                          player2.second.getRobot().getBody());
      if (collision) {
        collisions.push_back(player1.first);
      }
    }
  }

  // resolve all collisions
  for (auto &collision : collisions) {
    auto &player = players.at(collision);
    player.onCollision();
  }

  // check if any player is outside the arena
  for (auto &player : players) {
    Vector_d pos = player.second.getPosition();
    if (pos.x > rules.arena_size.x || pos.y > rules.arena_size.y || pos.x < 0 ||
        pos.y < 0) {
      player.second.onCollision();
    }
  }

  // remove players, that dont have health left.
  auto pred = [](const std::pair<const std::string, Player> &player) {
    return player.second.getHealth() <= 0;
  };
  auto it = players.begin();
  while ((it = std::find_if(it, players.end(), pred)) != players.end()) {
    players.erase(it++);
  }
}

void Simulation::addPlayer(std::string name, Player &player) {
  players.insert(KeyValuePair(name, std::move(player)));
}
void Simulation::newPlayer(std::string name, Agent *agent) {
  Player player(timeStep, rules);
  player.setAgent(agent);

  std::uniform_real_distribution<double> pos_x(0, rules.arena_size.x);
  std::uniform_real_distribution<double> pos_y(0, rules.arena_size.y);
  std::uniform_real_distribution<double> rot(0, 2 * M_PI);
  player.setPosition({pos_x(generator), pos_y(generator)});
  player.setRotation(rot(generator));

  addPlayer(name, player);
}

// void Simulation::drawRobot(sf::RenderTarget &target, sf::RenderStates states,
//                            const Robot &player) const {
//   // draw Body
//   Rectangle body = robot.getBody();
//
//   sf::RectangleShape rect({(float)body.getSize().x,
//   (float)body.getSize().y});
//   rect.setOrigin(0.5 * body.getSize().x, 0.5 * body.getSize().y);
//
//   double a = robot.getHealth() / 100.0;
//   if (a > 0)
//     rect.setFillColor({(uint8_t)(255 * (1 - a)), (uint8_t)(255 * a), 0});
//
//   target.draw(rect, states);
//   // NOTE: draw turret here
// }

// void Simulation::drawNameTag(sf::RenderTarget &target, sf::RenderStates
// states,
//                              const std::string &name) const {
//   sf::Text name_tag(name, font, 15);
//   name_tag.setOrigin(0.5 * name_tag.getLocalBounds().width, 0);
//   name_tag.setPosition({0, 15});
//
//   target.draw(name_tag, states);
// }

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

void Simulation::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  /*NOTE: This can be significantly simplified by using drawPlayer and calling
     drawRobot, drawLable and drawArc after modifiing states.transform
  */
  sf::RectangleShape rect(
      {(float)rules.arena_size.x, (float)rules.arena_size.y});
  rect.setFillColor(sf::Color::Transparent);
  rect.setOutlineColor(sf::Color::White);
  rect.setOutlineThickness(10);
  target.draw(rect, states);

  for (auto const &player : players) {
    target.draw(player.second, states);

    Vector_d p = player.second.getPosition();

    sf::Text name_tag(player.first, font, 15);
    name_tag.setOrigin(0.5 * name_tag.getLocalBounds().width, 0);
    name_tag.setPosition({(float)p.x, (float)p.y + 15});

    target.draw(name_tag, states);
    // drawArc(target, states, p, player.second.getRotation(), rules.scan_range,
    //         rules.scan_angle);
  }
}

bool Simulation::inSector(Vector_d const &p1, double rotation,
                          Vector_d const &p2) const {
  const double v_x = p2.x - p1.x;
  const double v_y = p2.y - p1.y;

  const double r = sqrt(pow(v_x, 2) + pow(v_y, 2));
  const bool in_range = r < rules.scan_range;

  const double alpha = atan2(v_y, v_x);
  const double beta = angDiffRadians(rotation, alpha);
  const bool in_segment =
      -rules.scan_angle / 2 < beta && beta < rules.scan_angle / 2;

  return in_range && in_segment;
}

void Simulation::check_scan(Player &player) {
  std::list<std::shared_ptr<Robot>> scanTargets;
  for (auto const &player2 : players) {
    if (&player == &player2.second) {
      continue;
    }
    Vector_d pose1 = player.getPosition();
    double rotation = player.getRotation();
    Vector_d pose2 = player2.second.getPosition();
    if (inSector(pose1, rotation, pose2)) {
      scanTargets.push_back(std::make_shared<Robot>(player2.second.getRobot()));
    }
  }
  player.setScanTargets(std::move(scanTargets));
}

bool Simulation::check_collision(Player &player) {
  for (auto const &player2 : players) {
    if (Collision::testCollision(player.getRobot().getBody(),
                                 player2.second.getRobot().getBody())) {
      return true;
    }
  }
  return false;
}
