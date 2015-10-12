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

  const double scan_range = 1500.0;
  const double scan_angle = M_PI / 12;

public:
  Simulation(sf::Font &font);
  // virtual ~Simulation() {}
  void update();
  void addPlayer(std::string name, Player &player);

private:
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const override;
  void drawArc(sf::RenderTarget &target, sf::RenderStates states, Pose pose,
               double radius, double angle) const;
  // void check_scan();
  bool inSector(Pose const &p1, Pose const &p2) const;
};

#endif /* end of include guard: __SIMULATION__ */
