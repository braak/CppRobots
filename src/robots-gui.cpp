/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file robots-gui.cpp
*   \author Jan-Niklas Braak
*/

#include "CppRobots.hpp"

// #include "Agents/Orbiter.hpp"
#include "Agents/Wanderer.hpp"
#include "Agents/Follower.hpp"

#include <math.h>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <memory>
#include <iostream>
#include <functional>

template <class URNG>
Pose randomPose(URNG &generator, double max_x, double max_y) {
  std::uniform_real_distribution<double> distribution_x(0, max_x);
  std::uniform_real_distribution<double> distribution_y(0, max_y);
  std::uniform_real_distribution<double> distribution_theta(0, M_PI * 2.0);
  return {distribution_x(generator), distribution_y(generator),
          distribution_theta(generator)};
}

/**
    This is the main function of the program.
    \return returns 0 on success
*/
int main() {
  constexpr double ZOOM_SPEED = 0.8;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rng(seed);

  std::vector<std::string> names = {"Albert", "Bob",   "Charlie",   "Daisy",
                                    "Eric",   "Frank", "Guinevere", "Hiro"};
  /*,      "Isabel",  "Julia",   "Kate",   "Ludwig", "Marge", "Nemo",
  "Oscar",  "Paige",   "Quentin", "Romeo",  "Stuart", "Tina",  "Usain",
  "Val",    "Wilhelm", "Xerxes",  "Yvonne", "Zack*/
  // Load resources
  sf::Font font;
  if (!font.loadFromFile("resources/font/liberation-fonts-ttf-2.00.1/"
                         "LiberationSans-Regular.ttf")) {
    throw std::runtime_error("unable to load font");
  }

  Simulation simulation(font);

  // create window
  std::stringstream window_name;
  window_name << "CppRobot Gui " << VERSION_SHORT;
  sf::RenderWindow window(sf::VideoMode::getDesktopMode(), window_name.str());
  window.setVerticalSyncEnabled(false);

  const double timeStep = 1.0 / 60.0;

  double zoom_level = 1;

  // create the players
  std::hash<std::string> string_hash;
  for (auto &name : names) {
    /*use name of player to generate psudo-random seed. So they behave the
    same each time.*/
    std::default_random_engine rng(string_hash(name));

    Player player(timeStep, {30, 18});
    // player.setAgent(randomOrbiter(rng, 60, 0.6));
    // player.setAgent(new Wanderer(string_hash(name), 0.6, 0.1, 60));
    player.setAgent(new Follower(100, 0.5, 0.1));
    player.setPose(randomPose(rng, window.getSize().x, window.getSize().y));

    simulation.addPlayer(name, player);
  }
  // Create the FrameTimer
  FrameTimer frameTimer(timeStep);

  // Create the fps_counter
  sf::Text fps_counter;
  fps_counter.setFont(font);
  fps_counter.setCharacterSize(10);

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
        double zoom = pow(ZOOM_SPEED, event.mouseWheel.delta);
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
