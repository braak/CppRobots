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

template <class Sim> struct Match {
  struct Player {
    std::function<Agent *()> agentFactory;
    int lives;
  };
  std::map<std::string, Player> players;
  std::shared_ptr<Simulation> simulation;
  int startingLives = 2;

  Match() : Match(Rules::defaultRules()){};
  Match(Rules rules)
      : Match(rules,
              std::chrono::system_clock::now().time_since_epoch().count()){};

  Match(Rules rules, std::size_t seed) {
    simulation = std::shared_ptr<Simulation>(new Sim(rules, seed));

    // onDeath = std::bind(&Match::_onDeath, this, _1);
    onDeath = [this](std::string name) { this->_onDeath(name); };
    simulation->deathSignal.connect(onDeath);
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
    // std::cout << name << " died! " << lives << " lives left" << std::endl;
    if (lives > 0) {
      simulation->newPlayer(name, players[name].agentFactory());
    } else {
      // std::cout << name << " lost " << simulation->getNumPlayers()
      //           << " players left" << std::endl;
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

  // unsigned int seed =
  //     std::chrono::system_clock::now().time_since_epoch().count();
  std::size_t seed = std::hash<std::string>()("Not Random");

  Match<SimulationSFML> match(rules, seed);

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
