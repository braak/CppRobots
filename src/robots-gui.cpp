/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file robots-gui.cpp
*   \author Jan-Niklas Braak
*/

#include "CppRobots.hpp"
#include "SimulationSFML.hpp"
#include "SimulationConsole.hpp"

#include "Agents/Orbiter.hpp"
#include "Agents/Wanderer.hpp"
#include "Agents/Follower.hpp"
#include "Agents/Sniper.hpp"
#include "Agents/Hunter.hpp"

#include <chrono>
#include <functional>
#include <fstream>

struct Match {
  struct Player {
    std::function<Agent *()> agentFactory;
    int lives;
  };
  std::map<std::string, Player> players;
  std::shared_ptr<Simulation> simulation;
  int startingLives = 2;

  Match(Simulation *sim) : simulation(sim) {
    onDeath = [this](std::string name) { this->_onDeath(name); };
    simulation->deathSignal.connect(onDeath);

    simulation->log("Welcome to CppRobots v" + std::string(VERSION_SHORT));
  };

  void addPlayer(std::string name, std::function<Agent *()> agentFactory) {
    Player player{agentFactory, startingLives};
    players.insert({name, player});
    simulation->newPlayer(name, agentFactory());
  }

  void run() {
    while (simulation->isRunning()) {
      simulation->update();
    }
  }

  Slot<std::string> onDeath;

private:
  void _onDeath(std::string name) {
    int lives = players[name].lives--;
    simulation->log(name + " died! " + std::to_string(lives) + " lives left");
    if (lives > 0) {
      simulation->newPlayer(name, players[name].agentFactory());
    } else {
      simulation->log(name + " lost! " +
                      std::to_string(simulation->getNumPlayers()) +
                      " players left");
    }
  };
};

/**
    This is the main function of the program.
    \return returns 0 on success
*/
int main() {
  Rules rules;

  std::ifstream inFile("Rules.json", std::ios::in);
  inFile >> rules;
  inFile.close();

  std::size_t seed = std::hash<std::string>()("Not Random");

  Match match(new SimulationSFML(rules, seed));

  auto names = {"Albert",    "Bob",  "Charlie", "Daisy", "Eric",    "Frank",
                "Guinevere", "Hiro", "Isabel",  "Julia", "Kate",    "Ludwig",
                "Marge",     "Nemo", "Oscar",   "Paige", "Quentin", "Romeo",
                "Stuart",    "Tina", "Usain",   "Val",   "Wilhelm", "Xerxes",
                "Yvonne",    "Zack"};

  auto hunterFactory = []() { return new Hunter(100, 20, 30); };
  //   // simulation.newPlayer(name, new Sniper());
  //   // simulation.newPlayer(name, new Follower(100, 100, 10));
  //   // simulation.newPlayer(name, new Orbiter(20, 0.6));
  for (auto &name : names) {
    match.addPlayer(name, hunterFactory);
  }
  match.run();

  return 0;
}
