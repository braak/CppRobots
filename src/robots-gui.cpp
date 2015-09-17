/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file main.cpp
*   \author Jan-Niklas Braak
*/

#include "version.h"
#include "Robot.hpp"
#include <sstream>
#include <SFML/Graphics.hpp>

/**
    This is the main function of the program.
    \param argc number of commandline parameters
    \param argv array of command line parameters
    \return returns 0 on success
*/
int main(int argc, char const *argv[]) {
  std::stringstream window_name;
  window_name << "CppRobot Gui v" << VERSION_MAJOR << "." << VERSION_MINOR
              << "." << VERSION_PATCH;
  sf::RenderWindow window(sf::VideoMode(800, 600), window_name.str());

  const double timeBase = 1.0 / 60;
  Robot robot(timeBase);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    robot.update(1, 0.1);

    window.clear(sf::Color::Black);

    // draw everything here...

    window.display();
  }
  return 0;
}
