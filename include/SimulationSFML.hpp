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

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <random>

class SimulationSFML : public Simulation, public sf::Drawable {
private:
  sf::Font font;

public:
  SimulationSFML(const Rules &rules, std::default_random_engine rng,
                 sf::Font font);

  // void run();

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
  void drawProjectile(sf::RenderTarget &target, sf::RenderStates states,
                      const Projectile &projectile) const;
  void drawArc(sf::RenderTarget &target, sf::RenderStates states,
               Vector_d position, double rotation, double radius,
               double angle) const;
  void drawPlayer(sf::RenderTarget &target, sf::RenderStates states,
                  const std::string &name, const Robot &robot) const;

  void drawRobot(sf::RenderTarget &target, sf::RenderStates states,
                 const Robot &robot) const;
};

#endif /* end of include guard: __SIMULATION_SFML__ */
