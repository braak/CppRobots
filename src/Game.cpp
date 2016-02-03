/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Game.cpp
*   \author Jan-Niklas Braak
*/

#include "Game.hpp"

Game::Game(Simulation *sim, View *view_) : simulation(sim), view(view_) {
  view->setSimulation(simulation);
  simulation->deathSignal.connect(_onDeath);
  simulation->simulationStepSignal.connect(_onSimulationStep);

  view->log("Welcome to CppRobots v" + std::string(VERSION_SHORT));
}

void Game::addPlayer(std::string name, std::function<Agent *()> agentFactory) {
  Player player{agentFactory, startingLives};
  players.insert({name, player});
  simulation->newPlayer(name, agentFactory());
}

void Game::run() {
  while (simulation->isRunning() && view->isRunning() && running) {
    view->input();
    simulation->update();
    view->output();
  }
  _onDeath.disconnect();
  _onSimulationStep.disconnect();
  view->finish();
}

void Game::onDeath(std::string name) {
  auto &lives = players[name].lives;

  lives--;
  if (lives > 0) {
    view->log(name + " died! " + std::to_string(lives) + " lives left");
    simulation->newPlayer(name, players[name].agentFactory());
  } else {
    view->log(name + " lost! " + std::to_string(simulation->getNumPlayers()) +
              " players left");
  }
}

void Game::onSimulationStep() {
  auto &players = simulation->getPlayers();
  if (players.size() <= 1) {
    if (!players.empty()) {
      std::string winner = players.begin()->first;
      view->log(winner + " wins the game!");
    }
    view->log("Game Over!");

    running = false;
  }
}
