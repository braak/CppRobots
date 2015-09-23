/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file robots-gui.cpp
*   \author Jan-Niklas Braak
*/
#include <math.h>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>

#include "CppRobots.hpp"

#include "Agents/Orbiter.hpp"

/**
    This is the main function of the program.
    \param argc number of commandline parameters
    \param argv array of command line parameters
    \return returns 0 on success
*/
int main() {
  // Load resources
  sf::Font font;
  if (!font.loadFromFile("resources/font/liberation-fonts-ttf-2.00."
                         "1/LiberationSans-Regular.ttf")) {
    exit(1);
  }

  // create window
  std::stringstream window_name;
  window_name << "CppRobot Gui v" << VERSION_SHORT;
  sf::RenderWindow window(sf::VideoMode(800, 600), window_name.str());
  window.setVerticalSyncEnabled(false);

  // create Game objects
  const double timeBase = 1.0 / 60.0;
  Robot robot(timeBase);
  robot.setPose(Pose(100, 100, 0));
  Agent *agent = new Orbiter(20, 0.6);

  FrameTimer frameTimer(timeBase);

  sf::Text fps_counter;
  fps_counter.setFont(font);
  fps_counter.setCharacterSize(10);

  while (window.isOpen()) {
    frameTimer.startFrame();
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    Robot::Action action = agent->update(robot);
    robot.update(action);

    Pose pose = robot.getPose();
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(10, 4));
    rectangle.setOutlineThickness(5);
    rectangle.setPosition(pose.x, pose.y);
    rectangle.setRotation(pose.theta * 180.0 / M_PI);

    fps_counter.setString(frameTimer.getOutput());

    window.clear(sf::Color::Black);

    window.draw(rectangle);
    window.draw(fps_counter);

    window.display();

    frameTimer.endFrame(true);
  }
  return 0;
}
