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

class SimulationSFML : public Simulation {
private:
  const std::string window_name = "CppRobot Gui " + std::string(VERSION_SHORT);
  const std::string fontName = "resources/font/liberation-fonts-ttf-2.00.1/"
                               "LiberationSans-Regular.ttf";
  sf::RenderWindow window;
  FrameTimer frameTimer;

  sf::Text fps_counter;
  sf::Font font;

  const double zoom_speed = 0.8;
  double zoom_level = 1;

public:
  SimulationSFML(const Rules &rules, std::default_random_engine rng);

  void update();

  void run() {
    while (window.isOpen()) {
      update();
    }
  }

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
