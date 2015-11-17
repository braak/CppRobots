/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file robots.cpp
*   \author Jan-Niklas Braak
*/

#include "CppRobots.hpp"
#include "SimulationConsole.hpp"
#include "Agents/Sniper.hpp"
#include "Agents/Hunter.hpp"
#include "Agents/Follower.hpp"
#include "Agents/Orbiter.hpp"

#include <chrono>
#include <functional>
#include <iostream>
#include <math.h>

/**
    This is the main function of the program.
    \return returns 0 on success
*/
int main() {
  // unsigned int seed =
  // std::chrono::system_clock::now().time_since_epoch().count();
  std::size_t seed = std::hash<std::string>()("Not Random");

  SimulationConsole simulation(Rules::defaultRules(), seed);

  auto names = {"Albert",    "Bob",  "Charlie", "Daisy", "Eric",    "Frank",
                "Guinevere", "Hiro", "Isabel",  "Julia", "Kate",    "Ludwig",
                "Marge",     "Nemo", "Oscar",   "Paige", "Quentin", "Romeo",
                "Stuart",    "Tina", "Usain",   "Val",   "Wilhelm", "Xerxes",
                "Yvonne",    "Zack"};
  // create the players
  for (auto &name : names) {
    /*simulation.newPlayer(name, new Wanderer(0.1, simulation.rules.v_max,
                                            std::hash<std::string>()(name)));*/
    // simulation.newPlayer(name, new Sniper());
    simulation.newPlayer(name, new Hunter(100, 20, 30));
    // simulation.newPlayer(name, new Follower(100, 100, 10));
    // simulation.newPlayer(name, new Orbiter(20, 0.6));
  }

  while (simulation.isRunning() && simulation.getRuntime() < 3 * 60 &&
         simulation.getNumPlayers() > 1) {
    simulation.update();
  }
  std::cout << "Simulation finished" << std::endl;
  return 0;
}
