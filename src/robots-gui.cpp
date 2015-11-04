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
  const double ZOOM_SPEED = 0.8;

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
  const double timeStep = simulation.rules.timeStep;

  // create window
  std::stringstream window_name;
  window_name << "CppRobot Gui " << VERSION_SHORT;
  sf::RenderWindow window(sf::VideoMode::getDesktopMode(), window_name.str());
  window.setVerticalSyncEnabled(false);

  sf::View view = window.getView();
  view.setCenter({(float)simulation.rules.arena_size.x / 2,
                  (float)simulation.rules.arena_size.y / 2});
  window.setView(view);

  double zoom_level = 1;

  // Create the FrameTimer
  FrameTimer frameTimer(timeStep);

  // Create the fps_counter
  sf::Text fps_counter;
  fps_counter.setFont(font);
  fps_counter.setCharacterSize(10);

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

  while (window.isOpen()) {
    frameTimer.startFrame();

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::Resized) {
        sf::View new_view = window.getView();
        new_view.setSize(static_cast<sf::Vector2f>(window.getSize()));
        new_view.zoom(zoom_level);
        window.setView(new_view);
      }
      if (event.type == sf::Event::MouseWheelMoved) {
        sf::View new_view = window.getView();
        const double zoom = pow(ZOOM_SPEED, event.mouseWheel.delta);
        new_view.zoom(zoom);
        zoom_level *= zoom;
        window.setView(new_view);
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Key::Escape) {
          window.close();
        }
      }
    }

    simulation.update();

    // Update the fps_counter
    fps_counter.setString(frameTimer.getOutput());

    // clear-draw-display cycle
    window.clear(sf::Color::Black);

    window.draw(simulation);

    sf::View old_view = window.getView();
    window.setView(sf::View({0.f, 0.f, static_cast<float>(window.getSize().x),
                             static_cast<float>(window.getSize().y)}));
    // Draw UI elements here
    window.draw(fps_counter);

    window.setView(old_view);

    window.display();

    frameTimer.endFrame(true);
  }
  return 0;
}
