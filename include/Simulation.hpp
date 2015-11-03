/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Simulation.cpp
*   \author Jan-Niklas Braak
*/
#ifndef __SIMULATION__
#define __SIMULATION__

#include "Robot.hpp"
#include "Agent.hpp"
#include "Angles.hpp"
#include "Collision.hpp"
#include "Rules.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>
#include <random>

class Simulation : public sf::Drawable {
public:
  const Rules rules;

private:
  // using KeyValuePair = std::pair<std::string, Player>;
  // std::map<std::string, Player> players;
  using KeyValuePair = std::pair<std::string, Robot>;
  std::map<std::string, Robot> players;

  sf::Font font;
  // const double timeStep;
  std::default_random_engine generator;

public:
  Simulation(sf::Font &font, std::default_random_engine rng,
             const Rules &rules);
  // virtual ~Simulation() {}
  void update();
  // void addPlayer(std::string name, Player &player);
  void newPlayer(std::string name, Agent *agent);
  void addPlayer(std::string name, Robot &player);

private:
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const override;
  void drawArc(sf::RenderTarget &target, sf::RenderStates states,
               Vector_d position, double rotation, double radius,
               double angle) const;
  void drawPlayer(sf::RenderTarget &target, sf::RenderStates states,
                  const std::string &name, const Robot &robot) const;

  void drawRobot(sf::RenderTarget &target, sf::RenderStates states,
                 const Robot &robot) const;

  void check_scan(Robot &robot);
  bool inSector(Vector_d const &p1, double rotation, Vector_d const &p2) const;
};

#endif /* end of include guard: __SIMULATION__ */
