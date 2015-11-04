/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file robots.cpp
*   \author Jan-Niklas Braak
*/

#include <iostream>
#include <math.h>

#include "CppRobots.hpp"
#include "Agents/Sniper.hpp"

class SimulationConsole : public Simulation {
  double runTime = 0;

public:
  SimulationConsole(const Rules &rules, std::default_random_engine rng)
      : Simulation(rules, rng) {
    std::cout << "Welcome to CppRobots v" << VERSION_SHORT << std::endl;
  };
  void update() {
    Simulation::update();

    runTime += rules.timeStep;
    std::cout << "runTime = " << int(runTime / 60) << ":" << fmod(runTime, 60)
              << std::endl;

    for (auto const &player : players) {
      std::cout << player.first << " at " << player.second.getPosition() << ", "
                << player.second.getHealth() << " health" << std::endl;
    }
  }
};

/**
    This is the main function of the program.
    \return returns 0 on success
*/
int main() {
  unsigned int seed =
      std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rng(seed);

  SimulationConsole simulation(Rules::defaultRules(), rng);

  std::vector<std::string> names = {
      "Albert", "Bob",     "Charlie", "Daisy",  "Eric",   "Frank", "Guinevere",
      "Hiro",   "Isabel",  "Julia",   "Kate",   "Ludwig", "Marge", "Nemo",
      "Oscar",  "Paige",   "Quentin", "Romeo",  "Stuart", "Tina",  "Usain",
      "Val",    "Wilhelm", "Xerxes",  "Yvonne", "Zack"};
  // create the players
  for (auto &name : names) {
    // simulation.newPlayer(name, new Wanderer(0.1, simulation.rules.v_max));
    simulation.newPlayer(name, new Sniper());
    // simulation.newPlayer(name, new Follower(100, 100, 10));
    // simulation.newPlayer(name, new Orbiter(20, 0.6));
  }

  for (size_t i = 0; i < 10000; i++) {
    simulation.update();
  }
  return 0;
}
