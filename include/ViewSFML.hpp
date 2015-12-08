/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file ViewSFML.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __VIEW_SFML__
#define __VIEW_SFML__

#include "Simulation.hpp"
#include "Rectangle.hpp"
#include "View.hpp"
#include "version.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <random>
#include <iostream>

/**
  A View that displays the state and progress of the simulation graphicly using
  the SFML library.
*/
class ViewSFML : public View {
private:
  const std::string window_name = "CppRobot Gui " + std::string(VERSION_SHORT);
  const std::string fontName = "resources/font/noto/NotoSans-Regular.ttf";
  const double zoom_speed = 0.8;

  double zoom_level = 1;
  int logIndex = 0;
  const int logLength = 70;

  std::shared_ptr<const Simulation> simulation;
  // Rules &rules;

  sf::RenderWindow window;
  std::vector<std::string> logging;
  sf::Font font;

public:
  ViewSFML();
  /**
    Constructor.
  */
  ViewSFML(Simulation *sim);

  void setSimulation(std::shared_ptr<const Simulation> sim) override;
  void input() override;
  void output() override;
  void finish() override;
  void log(std::string text) override;
  bool isRunning() const override;

private:
  void drawProjectile(sf::RenderTarget &target,
                      const Projectile &projectile) const;
  void drawPlayer(sf::RenderTarget &target, const std::string &name,
                  const Robot &robot) const;
  void drawArc(sf::RenderTarget &target, Vector_d position,
               double rotation) const;
  void drawLable(sf::RenderTarget &target, const std::string &name,
                 const Vector_d &position) const;
  void drawRobot(sf::RenderTarget &target, const Robot &robot) const;
  void drawUI(sf::RenderTarget &target) const;
  void drawArena(sf::RenderTarget &target) const;
};

#endif /* end of include guard: __VIEW_SFML__ */
