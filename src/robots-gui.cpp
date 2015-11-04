/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file robots-gui.cpp
*   \author Jan-Niklas Braak
*/

#include "CppRobots.hpp"
#include "SimulationSFML.hpp"

#include "Agents/Orbiter.hpp"
#include "Agents/Wanderer.hpp"
#include "Agents/Follower.hpp"
#include "Agents/Sniper.hpp"

#include <math.h>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <memory>
#include <iostream>
#include <functional>
#include <random>

/**
    This is the main function of the program.
    \return returns 0 on success
*/
int main() {
  unsigned int seed =
      std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rng(seed);

  // Load resources
  sf::Font font;
  if (!font.loadFromFile("resources/font/liberation-fonts-ttf-2.00.1/"
                         "LiberationSans-Regular.ttf")) {
    throw std::runtime_error("unable to load font");
  }

  SimulationSFML simulation(Rules::defaultRules(), rng, font);

  std::vector<std::string> names = {
      "Albert", "Bob",     "Charlie", "Daisy",  "Eric",   "Frank", "Guinevere",
      "Hiro",   "Isabel",  "Julia",   "Kate",   "Ludwig", "Marge", "Nemo",
      "Oscar",  "Paige",   "Quentin", "Romeo",  "Stuart", "Tina",  "Usain",
      "Val",    "Wilhelm", "Xerxes",  "Yvonne", "Zack"};

  // create the players
  for (auto &name : names) {
    // simulation.newPlayer(name, new Wanderer(0.1, simulation.rules.v_max));
    simulation.newPlayer(name, new Sniper());
    // simulation.newPlayer(name, new Follower(100, 100, 10));
    // simulation.newPlayer(name, new Orbiter(20, 0.6));
  }

  simulation.run();

  return 0;
}
