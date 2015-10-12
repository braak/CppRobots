/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Simulation.cpp
*   \author Jan-Niklas Braak
*/

#include "Simulation.hpp"

Simulation::Simulation(sf::Font &font) : font(font) {}

void Simulation::update() {
  for (auto &player : players) {
    // check_scan();

    std::list<std::shared_ptr<Robot>> scanTargets;
    for (auto const &player2 : players) {
      if (&player == &player2) {
        continue;
      }
      Pose pose1 = player.second.getPose();
      Pose pose2 = player2.second.getPose();
      if (inSector(pose1, pose2)) {
        scanTargets.push_back(
            std::make_shared<Robot>(player2.second.getRobot()));
      }
    }
    player.second.setScanTargets(std::move(scanTargets));

    player.second.update();
  }
}

void Simulation::addPlayer(std::string name, Player &player) {
  players.insert(KeyValuePair(name, std::move(player)));
}

void Simulation::drawArc(sf::RenderTarget &target, sf::RenderStates states,
                         Pose pose, double radius, double angle) const {
  sf::VertexArray lines(sf::TrianglesFan);
  lines.append({{(float)pose.x, (float)pose.y}, {255, 0, 0, 60}});

  for (double alpha = -angle; alpha <= angle; alpha += angle / 16) {
    float x = pose.x + radius * cos(pose.theta - alpha);
    float y = pose.y + radius * sin(pose.theta - alpha);
    lines.append({{x, y}, {255, 0, 0, 60}});
  }
  target.draw(lines, states);
}

void Simulation::draw(sf::RenderTarget &target, sf::RenderStates states) const {

  for (auto const &player : players) {
    target.draw(player.second, states);

    Pose p = player.second.getPose();

    sf::Text name_tag(player.first, font, 15);
    name_tag.setOrigin(0.5 * name_tag.getLocalBounds().width, 0);
    name_tag.setPosition({(float)p.x, (float)p.y + 15});

    target.draw(name_tag, states);
    drawArc(target, states, p, scan_range, scan_angle);

    // for (auto const &player2 : players) {
    //   Pose pose1 = player.second.getPose();
    //   Pose pose2 = player2.second.getPose();
    //   if (inSector(pose1, pose2)) {
    //     sf::VertexArray lines(sf::Lines);
    //     lines.append({{(float)pose1.x, (float)pose1.y}, {0, 255, 0, 60}});
    //     lines.append({{(float)pose2.x, (float)pose2.y}, {0, 255, 0, 60}});
    //     target.draw(lines, states);
    //   }
    // }
  }
}

bool Simulation::inSector(Pose const &p1, Pose const &p2) const {
  const double v_x = p2.x - p1.x;
  const double v_y = p2.y - p1.y;

  const double r = sqrt(pow(v_x, 2) + pow(v_y, 2));
  const bool in_range = r < scan_range;

  const double alpha = atan2(v_y, v_x);
  const double beta = angDiffRadians(p1.theta, alpha);
  const bool in_segment = -scan_angle < beta && beta < scan_angle;

  return in_range && in_segment;
}

//
// void Simulation::check_scan() {
//   for (auto &player1 : players) {
//     std::list<std::shared_ptr<Robot>> scanTargets;
//     for (auto &player2 : players) {
//       if (inSector(player1.second.getPose(), player2.second.getPose(),
//                    scan_range, scan_angle)) {
//         //&&  &player1.second != &player2.second
//
//         scanTargets.push_back(
//             std::make_shared<Robot>(player2.second.getRobot()));
//       }
//     }
//     player1.second.setScanTargets(scanTargets);
//   }
// }
