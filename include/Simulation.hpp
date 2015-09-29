/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Simulation.cpp
*   \author Jan-Niklas Braak
*/
#ifndef __SIMULATION__
#define __SIMULATION__

#include "Player.hpp"
#include "Angles.hpp"
#include <memory>

class Simulation : public sf::Drawable {
private:
  using KeyValuePair = std::pair<std::string, Player>;
  std::map<std::string, Player> players;
  sf::Font font;

public:
  Simulation() {
    if (!font.loadFromFile("resources/font/liberation-fonts-ttf-2.00.1/"
                           "LiberationSans-Regular.ttf")) {
      throw std::runtime_error("unable to load font");
    }
  }
  // virtual ~Simulation() {}
  void update() {
    for (auto &player : players) {
      player.second.update();
    }
  }
  void addPlayer(std::string name, Player &player) {
    players.insert(KeyValuePair(name, std::move(player)));
  }

  void check_scan() {
    for (auto &player1 : players) {
      std::list<std::shared_ptr<Robot>> scanTargets;
      for (auto &player2 : players) {
        if (inSector(player1.second.getPose(), player2.second.getPose(),
                     scan_range, scan_angle)) {
          //&&  &player1.second != &player2.second

          scanTargets.push_back(
              std::make_shared<Robot>(player2.second.getRobot()));
        }
      }
      player1.second.setScanTargets(scanTargets);
    }
  }

private:
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const override {

    for (auto const &player : players) {
      target.draw(player.second, states);

      Pose p = player.second.getPose();

      sf::Text name_tag(player.first, font, 15);
      name_tag.setOrigin(0.5 * name_tag.getLocalBounds().width, 0);
      name_tag.setPosition({(float)p.x, (float)p.y + 15});

      target.draw(name_tag, states);
    }
  }

  const double scan_range = 500.0;
  const double scan_angle = M_PI / 5;

  bool inSector(Pose const &p1, Pose const &p2, double radius, double angle) {
    const double v_x = p1.x - p2.x;
    const double v_y = p1.y - p2.y;

    const double r = sqrt(pow(v_x, 2) + pow(v_y, 2));
    const bool in_range = r < radius;

    const double alpha_min = angDiffRadians(p1.theta, angle);
    const double alpha_max = angDiffRadians(p1.theta + angle);
    const double beta = angDiffRadians(p1.theta, atan2(v_y, v_x));
    const bool in_segment = alpha_min < beta && beta < alpha_max;

    return in_range && in_segment;
  }
};

#endif /* end of include guard: __SIMULATION__ */
