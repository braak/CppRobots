/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Simulation.cpp
*   \author Jan-Niklas Braak
*/

#include "Simulation.hpp"

Simulation::Simulation(sf::Font &font, std::default_random_engine seed,
                       double timeStep_)
    : font(font), timeStep(timeStep_), generator(seed) {}

void Simulation::update() {
  for (auto &player : players) {
    check_scan(player.second);
  }
  for (auto &player : players) {
    player.second.update();
  }

  std::list<std::string> collisions;
  for (auto const &player1 : players) {
    for (auto const &player2 : players) {
      if (&player1 == &player2) {
        continue;
      }
      // NOTE: currently we check each pair of players twice.

      Collision collision(player1.second.getRobot().getBody(),
                          player2.second.getRobot().getBody());
      if (collision) {
        std::cout << player1.first << " collided with " << player2.first
                  << std::endl;
        collisions.push_back(player1.first);
      }
    }
  }

  for (auto &collision : collisions) {
    auto &player = players.at(collision);
    player.onCollision();
    if (player.getHealth() <= 0) {
      players.erase(collision);
    }
  }
}

void Simulation::addPlayer(std::string name, Player &player) {
  players.insert(KeyValuePair(name, std::move(player)));
}
void Simulation::newPlayer(std::string name, Agent *agent) {
  Player player(timeStep, robot_size);
  player.setAgent(agent);

  std::uniform_real_distribution<double> pos_x(0, arena_size.x);
  std::uniform_real_distribution<double> pos_y(0, arena_size.y);
  player.setPosition({pos_x(generator), pos_y(generator)});

  addPlayer(name, player);
}
void Simulation::drawArc(sf::RenderTarget &target, sf::RenderStates states,
                         Vector_d position, double rotation, double radius,
                         double angle) const {
  sf::VertexArray lines(sf::TrianglesFan);
  lines.append({{(float)position.x, (float)position.y}, {255, 0, 0, 60}});

  for (double alpha = -angle; alpha <= angle; alpha += angle / 16) {
    float x = position.x + radius * cos(rotation - alpha);
    float y = position.y + radius * sin(rotation - alpha);
    lines.append({{x, y}, {0, 0, 255, 60}});
  }
  target.draw(lines, states);
}

void Simulation::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  for (auto const &player : players) {
    target.draw(player.second, states);

    Vector_d p = player.second.getPosition();

    sf::Text name_tag(player.first, font, 15);
    name_tag.setOrigin(0.5 * name_tag.getLocalBounds().width, 0);
    name_tag.setPosition({(float)p.x, (float)p.y + 15});

    target.draw(name_tag, states);
    // drawArc(target, states, p, player.second.getRotation(), scan_range,
    //         scan_angle);
  }
}

bool Simulation::inSector(Vector_d const &p1, double rotation,
                          Vector_d const &p2) const {
  const double v_x = p2.x - p1.x;
  const double v_y = p2.y - p1.y;

  const double r = sqrt(pow(v_x, 2) + pow(v_y, 2));
  const bool in_range = r < scan_range;

  const double alpha = atan2(v_y, v_x);
  const double beta = angDiffRadians(rotation, alpha);
  const bool in_segment = -scan_angle < beta && beta < scan_angle;

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
