/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file robots.cpp
*   \author Jan-Niklas Braak
*/

#include "CppRobots.hpp"

#include "Agents/Orbiter.hpp"
#include "Agents/Wanderer.hpp"
#include "Agents/Follower.hpp"
#include "Agents/Sniper.hpp"
#include "Agents/Hunter.hpp"

#include <chrono>
#include <fstream>

/**
    This is the main function of the program.
    \return returns 0 on success
*/
int main() {
  Rules rules;

  std::ifstream inFile(selfpath() + "/Rules.json", std::ios::in);
  if (inFile.is_open()) {
    inFile >> rules;
    inFile.close();
  } else {
    std::cout << "Unable to load Rules.json. Using defaults instead."
              << std::endl;
    rules = Rules::defaultRules();
  }

  std::size_t seed = std::hash<std::string>()("Not Random");

#ifdef USE_SFML
  Game game(new SimulationSFML(rules, seed));
#else
  Game game(new SimulationConsole(rules, seed));
#endif

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
    game.addPlayer(name, hunterFactory);
  }
  game.run();

  return 0;
}
