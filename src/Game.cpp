/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Game.hpp
*   \author Jan-Niklas Braak
*/

#include "Game.hpp"

Game::Game(Simulation *sim) : simulation(sim) {
  _onDeath = [this](std::string name) { this->onDeath(name); };
  simulation->deathSignal.connect(_onDeath);

  _onSimulationStep = [this]() { this->onSimulationStep(); };
  simulation->simulationStepSignal.connect(_onSimulationStep);

  simulation->log("Welcome to CppRobots v" + std::string(VERSION_SHORT));
}

void Game::addPlayer(std::string name, std::function<Agent *()> agentFactory) {
  Player player{agentFactory, startingLives};
  players.insert({name, player});
  simulation->newPlayer(name, agentFactory());
}

void Game::run() {
  while (simulation->isRunning() && running) {
    simulation->update();
  }
  _onDeath.disconnect();
  _onSimulationStep.disconnect();
  simulation->finish();
}

void Game::onDeath(std::string name) {
  int lives = players[name].lives--;
  if (lives > 0) {
    simulation->log(name + " died! " + std::to_string(lives) + " lives left");
    simulation->newPlayer(name, players[name].agentFactory());
  } else {
    simulation->log(name + " lost! " +
                    std::to_string(simulation->getNumPlayers()) +
                    " players left");
  }
}

void Game::onSimulationStep() {
  if (simulation->getNumPlayers() <= 1) {
    auto &players = simulation->getPlayers();
    if (!players.empty()) {
      std::string winner = players.begin()->first;
      simulation->log(winner + " wins the game!");
    }
    simulation->log("Game Over!");

    running = false;
  }
}
