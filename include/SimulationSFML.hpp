/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file SimulationSFML.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __SIMULATION_SFML__
#define __SIMULATION_SFML__

#include "Simulation.hpp"
#include "Rectangle.hpp"
#include "FrameTimer.hpp"
#include "version.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <random>
#include <iostream>

/**
  Simulation, that displays its results graphicly using the SFML library.
*/
class SimulationSFML : public Simulation {
private:
  const std::string window_name = "CppRobot Gui " + std::string(VERSION_SHORT);
  const std::string fontName = "resources/font/noto/NotoSans-Regular.ttf";
  sf::RenderWindow window;
  FrameTimer frameTimer;

  sf::Text fps_counter;
  sf::Font font;

  const double zoom_speed = 0.8;
  double zoom_level = 1;

  std::vector<std::string> logging;
  int logIndex = 0;
  const int logLength = 70;

public:
  /**
    Constructor.

    \param rules Rules used in the Simulation.
    \param seed Seed used for random number generation.
  */
  SimulationSFML(const Rules &rules, unsigned int seed);

  /**
    Run one step of the simmulation.
  */
  void update() override;
  /**
    Return whether the Simulation is currently running.
    The Simulation runns aslong as its Window is open.
    \return whether the Simulation is currently running.
  */
  bool isRunning() const override;
  /**
    Finishes the simulation by waiting for the user to close the window.
  */
  void finish() override;
  /**
    Add a line of text to the log.

    The log is displayed in the window.
    \param text Line of text to add to the log.
  */
  void log(std::string text) override;

private:
  void drawProjectile(sf::RenderTarget &target,
                      const Projectile &projectile) const;
  void drawArc(sf::RenderTarget &target, Vector_d position,
               double rotation) const;
  void drawPlayer(sf::RenderTarget &target, const std::string &name,
                  const Robot &robot) const;
  void drawLable(sf::RenderTarget &target, const std::string &name,
                 const Vector_d &position) const;
  void drawRobot(sf::RenderTarget &target, const Robot &robot) const;

  void drawUI(sf::RenderTarget &target) const;
  void drawArena(sf::RenderTarget &target) const;
};

#endif /* end of include guard: __SIMULATION_SFML__ */
