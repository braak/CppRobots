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

class SimulationConsole : public Simulation {

public:
  SimulationConsole(const Rules &rules, unsigned int seed)
      : Simulation(rules, seed) {
    std::cout << "Welcome to CppRobots v" << VERSION_SHORT << std::endl;
  };
  void update() {
    Simulation::update();

    std::cout << "runTime = " << int(runTime / 60) << ":" << fmod(runTime, 60)
              << std::endl;

    for (auto const &player : players) {
      std::cout << player.first << " at " << player.second.getPosition() << ", "
                << player.second.getHealth() << " health" << std::endl;
    }
  }
};

#endif /* end of include guard: __SIMULATION_CONSOLE__ */
