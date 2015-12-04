/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file SimulationConsole.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __SIMULATION_CONSOLE__
#define __SIMULATION_CONSOLE__

#include "Simulation.hpp"

#include <iostream>
#include <string>

/**
  Simulation, that prints its rsults to the console.
*/
class SimulationConsole : public Simulation {

public:
  /**
    Constructor.

    \param rules Rules used in the Simulation.
    \param seed Seed used for random number generation.
  */
  SimulationConsole(const Rules &rules, unsigned int seed)
      : Simulation(rules, seed){};
  /**
    Run one step of the simmulation.
  */
  void update() {
    Simulation::update();
    std::cout << runtimeString() << std::endl;
    for (auto const &player : players) {
      std::cout << "\t" << player.first << " at " << player.second.getPosition()
                << ", " << player.second.getHealth() << " health" << std::endl;
    }
  }
  /**
    Prints a line of text to the console.
    \param text Line of text to print to the console.
  */
  void log(std::string text) override {
    std::cout << runtimeString() << ": " << text << std::endl;
  }
  /**
    Finishes the simulation by waitmg for user input.
  */
  virtual void finish() override {
    // wait for user input
    std::cin.get();
  }
};

#endif /* end of include guard: __SIMULATION_CONSOLE__ */
