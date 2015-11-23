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
      : Simulation(rules, seed){};
  void update() {
    Simulation::update();
    std::cout << runtimeString() << std::endl;
    for (auto const &player : players) {
      // std::stringstream out;
      std::cout << "\t" << player.first << " at " << player.second.getPosition()
                << ", " << player.second.getHealth() << " health" << std::endl;
      // log(out.str());
    }
  }
  void log(std::string text) override {
    std::cout << runtimeString() << ": " << text << std::endl;
  }
  virtual void finsish() override { std::cin.get(); }
};

#endif /* end of include guard: __SIMULATION_CONSOLE__ */
