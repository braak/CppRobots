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
  }
}

// bool Simulation::inSector(Pose const &p1, Pose const &p2, double radius,
//                           double angle) {
//   const double v_x = p1.x - p2.x;
//   const double v_y = p1.y - p2.y;
//
//   const double r = sqrt(pow(v_x, 2) + pow(v_y, 2));
//   const bool in_range = r < radius;
//
//   const double alpha_min = angDiffRadians(p1.theta, angle);
//   const double alpha_max = angDiffRadians(p1.theta + angle);
//   const double beta = angDiffRadians(p1.theta, atan2(v_y, v_x));
//   const bool in_segment = alpha_min < beta && beta < alpha_max;
//
//   return in_range && in_segment;
// }
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
