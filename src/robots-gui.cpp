/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file main.cpp
*   \author Jan-Niklas Braak
*/

#include "version.h"
#include "Robot.hpp"
#include <math.h>
#include <sstream>
#include <SFML/System.hpp>
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
  window.setVerticalSyncEnabled(false);

  const double timeBase = 1.0 / 60;
  Robot robot(timeBase);
  robot.setPose(Pose(100, 100, 0));

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    robot.update(20, 0.6);
    Pose pose = robot.getPose();
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(10, 4));
    rectangle.setOutlineThickness(5);
    rectangle.setPosition(pose.x, pose.y);
    rectangle.setRotation(pose.theta * 180.0 / M_PI);

    window.clear(sf::Color::Black);

    window.draw(rectangle);

    window.display();

    sf::sleep(sf::seconds(timeBase));
  }
  return 0;
}
