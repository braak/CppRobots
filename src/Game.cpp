/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Game.cpp
*   \author Jan-Niklas Braak
*/

#include "Game.hpp"

Game::Game(Simulation *sim, View *view_) : simulation(sim), view(view_) {
  // setup the View
  view->setSimulation(simulation);
  // call _onDeath every time a Robot dies
  simulation->deathSignal.connect(_onDeath);
  // call _onSimulationStep every step of the simulation
  simulation->simulationStepSignal.connect(_onSimulationStep);

  view->log("Welcome to CppRobots v" + std::string(VERSION_SHORT));
}

void Game::addPlayer(std::string name, std::function<Agent *()> agentFactory) {
  // create and remember Player
  Player player{agentFactory, startingLives};
  players.insert({name, player});
  // create a Robot for the Player
  simulation->newPlayer(name, agentFactory());
}

void Game::run() {
  while (simulation->isRunning() && view->isRunning() && running) {
    view->input();
    simulation->update();
    view->output();
  }
  // disconect slots to make sure thex are no longer triggert.
  _onDeath.disconnect();
  _onSimulationStep.disconnect();
  // Perform some cleanup for the view
  view->finish();
}

void Game::onDeath(std::string name) {
  auto &lives = players[name].lives;

  lives--;
  if (lives > 0) {
    // Respawn the Player.
    view->log(name + " died! " + std::to_string(lives) + " lives left");
    simulation->newPlayer(name, players[name].agentFactory());
  } else {
    view->log(name + " lost! " + std::to_string(simulation->getNumPlayers()) +
              " players left");
  }
}

void Game::onSimulationStep() {
  // Check if the game is over.
  auto &players = simulation->getPlayers();
  if (players.size() <= 1) {
    // The game is over Who won?
    if (!players.empty()) {
      std::string winner = players.begin()->first;
      view->log(winner + " wins the game!");
    }
    view->log("Game Over!");

    running = false;
  }
  // NOTE: checking every step of the simulation is inefficient. We cloud
  // check during the onDeath signal.
}
