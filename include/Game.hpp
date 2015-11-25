/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Game.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __CPPROBOTS_GAME__
#define __CPPROBOTS_GAME__

#include "SignalSlot.hpp"
#include "Simulation.hpp"

#include <functional>
#include <map>
#include <memory>

class Game {
  struct Player {
    std::function<Agent *()> agentFactory;
    int lives;
  };

  std::map<std::string, Player> players;
  std::shared_ptr<Simulation> simulation;

  Slot<std::string> _onDeath;

  int startingLives = 2;
  bool running = true;

public:
  Game(Simulation *sim) : simulation(sim) {
    _onDeath = [this](std::string name) { this->onDeath(name); };
    simulation->deathSignal.connect(_onDeath);

    simulation->log("Welcome to CppRobots v" + std::string(VERSION_SHORT));
  }

  void addPlayer(std::string name, std::function<Agent *()> agentFactory) {
    Player player{agentFactory, startingLives};
    players.insert({name, player});
    simulation->newPlayer(name, agentFactory());
  }

  void run() {
    while (simulation->isRunning() && running) {
      simulation->update();
    }
    simulation->finish();
  }

  virtual void onDeath(std::string name) {
    int lives = players[name].lives--;
    if (lives > 0) {
      simulation->log(name + " died! " + std::to_string(lives) + " lives left");
      simulation->newPlayer(name, players[name].agentFactory());
    } else {
      simulation->log(name + " lost! " +
                      std::to_string(simulation->getNumPlayers()) +
                      " players left");
    }
    if (simulation->getNumPlayers() <= 1) {
      std::string winner = simulation->getPlayers().begin()->first;
      simulation->log(winner + " wins the game!");
      simulation->log("Game Over!");

      running = false;
    }
  };
};

#endif /* end of include guard: __CPPROBOTS_GAME__ */
