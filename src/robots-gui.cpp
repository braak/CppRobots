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

template <class URNG>
Pose randomPose(URNG &generator, long unsigned int max_x,
                long unsigned int max_y) {
  std::uniform_int_distribution<long unsigned int> distribution_x(0, max_x);
  std::uniform_int_distribution<long unsigned int> distribution_y(0, max_y);
  std::uniform_real_distribution<double> distribution_theta(0, M_PI * 2.0);
  return {distribution_x(generator), distribution_y(generator),
          distribution_theta(generator)};
}
template <class URNG>
Agent *randomOrbiter(URNG &generator, double max_speed,
                     double max_turning_rate) {
  std::uniform_real_distribution<double> distribution_v(0, max_speed);
  std::uniform_real_distribution<double> distribution_w(-max_turning_rate,
                                                        max_turning_rate);
  return new Orbiter(distribution_v(generator), distribution_w(generator));
}

/**
    This is the main function of the program.
    \return returns 0 on success
*/
int main() {
  std::default_random_engine rng;
  std::vector<Player> players;
  // Load resources
  sf::Font font;
  if (!font.loadFromFile("resources/font/liberation-fonts-ttf-2.00.1/"
                         "LiberationSans-Regular.ttf")) {
    exit(1);
  }

  // create window
  std::stringstream window_name;
  window_name << "CppRobot Gui " << VERSION_SHORT;
  sf::RenderWindow window(sf::VideoMode(800, 600), window_name.str());
  window.setVerticalSyncEnabled(false);

  const double timeStep = 1.0 / 60.0;

  // Create the players
  for (size_t i = 0; i < 20; i++) {
    Player player(timeStep, {30, 18});
    player.setPose(randomPose(rng, window.getSize().x, window.getSize().y));
    player.setAgent(randomOrbiter(rng, 60, 0.6));

    players.push_back(std::move(player));
  }

  // Create the FrameTimer
  FrameTimer frameTimer(timeStep);

  // Create the fps-counter
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

    //  Update the Player(s)
    for (auto &player : players) {
      player.update();
    }

    // Update the fps_counter
    fps_counter.setString(frameTimer.getOutput());

    // clear-draw-display cycle
    window.clear(sf::Color::Black);

    for (auto const &player : players) {
      window.draw(player);
    }

    window.draw(fps_counter);

    window.display();

    frameTimer.endFrame(true);
  }
  return 0;
}
