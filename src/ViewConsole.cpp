/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file ViewConsole.cpp
*   \author Jan-Niklas Braak
*/

#include "ViewConsole.hpp"

void ViewConsole::setSimulation(std::shared_ptr<const Simulation> sim) {
  simulation = sim;
}

void ViewConsole::input() {}
void ViewConsole::output() {
  if (!simulation) {
    throw std::runtime_error("No Simulation was set for this View.");
  }
  std::cout << simulation->runtimeString() << std::endl;
  for (auto const &player : simulation->getPlayers()) {
    std::cout << "\t" << player.first << " at " << player.second.getPosition()
              << ", " << player.second.getHealth() << " health" << std::endl;
  }
}
void ViewConsole::finish() {
  std::cout << "Game Over!" << std::endl
            << "Press enter to quit.";
  std::cin.get();
}
bool ViewConsole::isRunning() const { return true; }
void ViewConsole::log(std::string text) {
  if (!simulation) {
    throw std::runtime_error("No Simulation was set for this View.");
  }
  std::cout << simulation->runtimeString() << ": " << text << std::endl;
}
