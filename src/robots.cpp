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
#include <algorithm>

/**
    This is the main function of the program.
    \return returns 0 on success
*/
int main() {
  Rules rules;
  Json::Value names;

  std::ifstream inFile(selfpath() + "/config/Rules.json", std::ios::in);
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

  std::ifstream nameFile(selfpath() + "/config/Names.json", std::ios::in);
  if (!nameFile.is_open()) {
    throw std::runtime_error("unable to load Names.json");
  } else {
    nameFile >> names;
    if (!names.isArray()) {
      throw std::runtime_error("Names.json is not an array of names");
    }
    nameFile.close();
  }

  auto hunterFactory = []() { return new Hunter(100, 20, 30); };
  for (auto const &name : names) {
    game.addPlayer(name.asString(), hunterFactory);
  }

  game.run();

  return 0;
}
