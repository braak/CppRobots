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
  Simulation, that displays its results graphicly using the SFML library.
*/
class ViewSFML : public View {
private:
  const std::string window_name = "CppRobot Gui " + std::string(VERSION_SHORT);
  const std::string fontName = "resources/font/noto/NotoSans-Regular.ttf";
  const double zoom_speed = 0.8;

  double zoom_level = 1;
  int logIndex = 0;
  const int logLength = 70;

  std::shared_ptr<Simulation> simulation;
  Rules &rules;

  sf::RenderWindow window;
  std::vector<std::string> logging;
  sf::Font font;

public:
  ViewSFML(Rules &rules_) : ViewSFML(nullptr, rules_) {}
  /**
    Constructor.
  */
  ViewSFML(Simulation *sim, Rules &rules_)
      : simulation(sim), rules(rules_),
        window(sf::VideoMode::getDesktopMode(), window_name),
        logging(logLength) {
    // center the view on the Arena
    sf::View view = window.getView();
    view.setCenter(
        {(float)rules.arena_size.x / 2, (float)rules.arena_size.y / 2});
    window.setView(view);

    // Load resources
    if (!font.loadFromFile(selfpath() + "/" + fontName)) {
      throw std::runtime_error("unable to load font");
    }
  }

  // set artibutes  }
  void setSimulation(std::shared_ptr<Simulation> sim) override {
    simulation = sim;
  }

  void input() override {
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
        const double zoom = pow(zoom_speed, event.mouseWheel.delta);
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
  }

  void output() override {
    if (!simulation) {
      throw std::runtime_error("No Simulation was set for this View.");
    }

    /*NOTE: to get better perfomance for drawing only create only create the
     * Graphical objets once and update their state here.
    */

    // clear-draw-display cycle
    window.clear(sf::Color::Black);

    // draw Arena
    drawArena(window);

    // draw all players
    for (auto const &player : simulation->getPlayers()) {
      drawPlayer(window, player.first, player.second);
    }
    // draw all projectiles
    for (auto const &projectile : simulation->getProjectiles()) {
      drawProjectile(window, projectile);
    }
    // draw UI
    drawUI(window);

    window.display();
  }

  void finish() override {
    while (isRunning()) {
      input();
      window.clear(sf::Color::Black);

      sf::Text game_over_text("Game Over ", font, 32);
      game_over_text.setPosition(
          {(float)rules.arena_size.x / 2, (float)rules.arena_size.y / 2});
      game_over_text.setOrigin(0.5 * game_over_text.getLocalBounds().width, 0);
      window.draw(game_over_text);

      drawUI(window);

      window.display();
    }
  }
  void log(std::string text) override {
    logging[logIndex++ % logLength] = simulation->runtimeString() + ": " + text;
  }

  bool isRunning() const { return window.isOpen(); }

private:
  void drawProjectile(sf::RenderTarget &target,
                      const Projectile &projectile) const {
    Rectangle body = projectile.getBody();

    sf::RectangleShape rect({(float)body.getSize().x, (float)body.getSize().y});
    rect.setOrigin(0.5 * body.getSize().x, 0.5 * body.getSize().y);
    rect.setPosition(
        {(float)body.getPosition().x, (float)body.getPosition().y});
    rect.setRotation(degrees(body.getRotation()));
    target.draw(rect);
  }
  void drawPlayer(sf::RenderTarget &target, const std::string &name,
                  const Robot &robot) const {
    // drawRobot
    drawRobot(target, robot);

    // drawLable
    drawLable(target, name, robot.getPosition());

    // drawArc(target, robot.getPosition(),
    //         robot.getRotation() + robot.getTurretAngle());
  }
  void drawArc(sf::RenderTarget &target, Vector_d position,
               double rotation) const {
    double angle = rules.scan_angle;
    double radius = rules.scan_range;
    /* NOTE: this function is very inefficient. It can be improved by having a
    constant Arc object and modifiing its position via the transformation
    matrix of RenderStates acording to the player position.
    */
    sf::VertexArray lines(sf::TrianglesFan);
    lines.append({{(float)position.x, (float)position.y}, {0, 0, 255, 60}});

    const size_t numPoints = 16;
    for (size_t i = 0; i < numPoints; i++) {
      // angle*(i/numPoints - 0.5)
      double alpha = -angle / 2 + i * angle / numPoints;
      float x = position.x + radius * cos(rotation - alpha);
      float y = position.y + radius * sin(rotation - alpha);
      lines.append({{x, y}, {0, 0, 255, 60}});
    }
    target.draw(lines);
  }
  void drawLable(sf::RenderTarget &target, const std::string &name,
                 const Vector_d &position) const {
    sf::Text name_tag(sf::String::fromUtf8(name.begin(), name.end()), font, 15);
    name_tag.setOrigin(0.5 * name_tag.getLocalBounds().width, 0);
    name_tag.setPosition({(float)position.x, (float)position.y + 15});

    target.draw(name_tag);
  }
  void drawRobot(sf::RenderTarget &target, const Robot &robot) const {
    Rectangle body = robot.getBody();

    sf::RectangleShape rect({(float)body.getSize().x, (float)body.getSize().y});
    rect.setOrigin(0.5 * body.getSize().x, 0.5 * body.getSize().y);
    rect.setPosition(
        {(float)body.getPosition().x, (float)body.getPosition().y});
    rect.setRotation(degrees(body.getRotation()));
    double a = robot.getHealth() / 100.0;
    if (a > 0)
      rect.setFillColor({(uint8_t)(255 * (1 - a)), (uint8_t)(255 * a), 0});
    target.draw(rect);

    sf::RectangleShape turret(
        {(float)body.getSize().x, (float)body.getSize().y / 3});
    turret.setPosition(
        {(float)body.getPosition().x, (float)body.getPosition().y});
    turret.setRotation(degrees(body.getRotation() + robot.getTurretAngle()));
    turret.setOrigin(0.5 * turret.getSize().y, 0.5 * turret.getSize().y);

    target.draw(turret);
  }

  void drawUI(sf::RenderTarget &target) const {
    sf::View old_view = target.getView();
    target.setView(sf::View({0.f, 0.f, static_cast<float>(target.getSize().x),
                             static_cast<float>(target.getSize().y)}));

    const int spacing = 13;
    sf::Text log_line("", font, 12);
    log_line.move({0, spacing});
    for (int i = 0; i < logLength; i++) {
      const std::string &line = logging[(logIndex + i) % logLength];
      if (!line.empty()) {
        log_line.setString(sf::String::fromUtf8(line.begin(), line.end()));
        log_line.move({0, spacing});

        target.draw(log_line);
      }
    }
    target.setView(old_view);
  }
  void drawArena(sf::RenderTarget &target) const {
    sf::RectangleShape rect(
        {(float)rules.arena_size.x, (float)rules.arena_size.y});
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(5);
    target.draw(rect);
  }
};

#endif /* end of include guard: __VIEW_SFML__ */
