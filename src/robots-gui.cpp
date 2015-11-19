/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file robots-gui.cpp
*   \author Jan-Niklas Braak
*/

#include "CppRobots.hpp"
#include "Simulation.hpp"
#include "SimulationSFML.hpp"
// #include "SimulationConsole.hpp"

#include "Agents/Orbiter.hpp"
#include "Agents/Wanderer.hpp"
#include "Agents/Follower.hpp"
#include "Agents/Sniper.hpp"
#include "Agents/Hunter.hpp"

#include <chrono>
#include <functional>
#include <fstream>

#include "json/json.h"

using namespace std::placeholders;

struct Player {
  std::function<Agent *()> agentFactory;
  int lives;
};

// struct Match {
//   std::map<std::string, Player> players;
//   std::shared_ptr<Simulation> simulation;
//
//   Match() : onDeath(std::bind(&Match::_onDeath, this, _1)) {
//     std::size_t seed = std::hash<std::string>()("Not Random");
//
//     simulation = std::shared_ptr<Simulation>(
//         new SimulationSFML(Rules::defaultRules(), seed));
//
//     simulation->deathSignal.connect(onDeath);
//   };
// void run(){
//   while (simulation->isRunning()) {
//     simulation->update();
//   }
// }
//
//   Slot<std::string> onDeath;
//
// private:
//   void _onDeath(std::string name) {
//     int lives = players[name].lives--;
//     std::cout << name << " died! " << lives << " lives left" << std::endl;
//     if (lives > 0) {
//       simulation->newPlayer(name, players[name].agentFactory());
//     } else {
//       std::cout << name << " lost " << simulation->getNumPlayers()
//                 << " players left" << std::endl;
//     }
//   };
// };

/**
    This is the main function of the program.
    \return returns 0 on success
*/
int main() {
  Rules rules;
  std::ifstream inFile("Rules.json", std::ios::in);
  inFile >> rules;
  inFile.close();

  // std::cout << rules << std::endl;
  // unsigned int seed =
  //     std::chrono::system_clock::now().time_since_epoch().count();
  std::size_t seed = std::hash<std::string>()("Not Random");

  std::shared_ptr<Simulation> simulation =
      std::shared_ptr<Simulation>(new SimulationSFML(rules, seed));

  auto names = {"Albert",    "Bob",  "Charlie", "Daisy", "Eric",    "Frank",
                "Guinevere", "Hiro", "Isabel",  "Julia", "Kate",    "Ludwig",
                "Marge",     "Nemo", "Oscar",   "Paige", "Quentin", "Romeo",
                "Stuart",    "Tina", "Usain",   "Val",   "Wilhelm", "Xerxes",
                "Yvonne",    "Zack"};

  std::map<std::string, Player> players;
  for (auto &name : names) {
    players[name] = {[]() { return new Hunter(100, 20, 30); }, 10};
    simulation->newPlayer(name, players[name].agentFactory());
  }
  // // create the players
  // for (auto &name : names) {
  //   /*simulation.newPlayer(name, new Wanderer(0.1, simulation.rules.v_max,
  //                                           std::hash<std::string>()(name)));*/
  //   // simulation.newPlayer(name, new Sniper());
  //   // simulation.newPlayer(name, new Follower(100, 100, 10));
  //   // simulation.newPlayer(name, new Orbiter(20, 0.6));
  // }

  // simulation.newPlayer("Hunter", new Hunter(100, 100, 30));
  // simulation.newPlayer("Target", new Wanderer(0, 0),
  //                      simulation.rules.arena_size / 2.0, 0.0);

  Slot<std::string> playerDeath(
      [simulation, &players](std::string name) mutable {
        int lives = players[name].lives--;
        std::cout << name << " died! " << lives << " lives left" << std::endl;
        if (lives > 0) {
          simulation->newPlayer(name, new Hunter(100, 20, 30));
        } else {
          std::cout << name << " lost " << simulation->getNumPlayers()
                    << " players left" << std::endl;
        }
      });

  simulation->deathSignal.connect(playerDeath);
  while (simulation->isRunning()) {
    simulation->update();
  }

  return 0;
}
