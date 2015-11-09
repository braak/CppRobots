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
#include "Projectile.hpp"

#include <memory>
#include <iostream>
#include <random>
#include <list>
#include <map>

class Simulation {
public:
  const Rules rules;

protected:
  using KeyValuePair = std::pair<std::string, Robot>;
  std::map<std::string, Robot> players;
  std::list<Projectile> projectiles;

  std::default_random_engine generator;

public:
  Simulation(const Rules &rules, std::default_random_engine rng);
  void newPlayer(std::string name, Agent *agent);
  void update();

private:
  void updatePlayers();
  void updateProjectiles();

  void check_scan(Robot &robot);
  bool inSector(Vector_d const &p1, double rotation, Vector_d const &p2) const;
};

#endif /* end of include guard: __SIMULATION__ */
