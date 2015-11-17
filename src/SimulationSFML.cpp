/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file SimulationSFML.cpp
*   \author Jan-Niklas Braak
*/

#include "SimulationSFML.hpp"

SimulationSFML::SimulationSFML(const Rules &rules, unsigned int seed)
    : Simulation(rules, seed),
      window(sf::VideoMode::getDesktopMode(), window_name),
      frameTimer(rules.timeStep) {

  // center the view on the Arena
  sf::View view = window.getView();
  view.setCenter(
      {(float)rules.arena_size.x / 2, (float)rules.arena_size.y / 2});
  window.setView(view);

  // Load resources
  if (!font.loadFromFile(fontName)) {
    if (!font.loadFromFile("bin/" + fontName)) {
      throw std::runtime_error("unable to load font");
    }
  }
  std::cout << "Font \"" << fontName << "\" loaded " << std::endl;

  // set static artibutes
  fps_counter.setFont(font);
  fps_counter.setCharacterSize(10);
}

void SimulationSFML::update() {

  frameTimer.startFrame(true);

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

  Simulation::update();
  /*NOTE: to get better perfomance for drawing only create only create the
   * Graphical objets once and update their state here.
  */

  // Update the fps_counter
  fps_counter.setString(frameTimer.getOutput());

  // clear-draw-display cycle
  window.clear(sf::Color::Black);

  // draw Arena
  drawArena(window);

  // draw all players
  for (auto const &player : players) {
    drawPlayer(window, player.first, player.second);
  }
  // darw all projectiles
  for (auto const &projectile : projectiles) {
    drawProjectile(window, projectile);
  }
  // draw UI
  drawUI(window);

  window.display();

  frameTimer.endFrame();
}

bool SimulationSFML::isRunning() const {
  return Simulation::isRunning() && window.isOpen();
}

void SimulationSFML::drawProjectile(sf::RenderTarget &target,
                                    const Projectile &projectile) const {
  Rectangle body = projectile.getBody();

  sf::RectangleShape rect({(float)body.getSize().x, (float)body.getSize().y});
  rect.setOrigin(0.5 * body.getSize().x, 0.5 * body.getSize().y);
  rect.setPosition({(float)body.getPosition().x, (float)body.getPosition().y});
  rect.setRotation(degrees(body.getRotation()));
  target.draw(rect);
}
void SimulationSFML::drawRobot(sf::RenderTarget &target,
                               const Robot &robot) const {
  Rectangle body = robot.getBody();

  sf::RectangleShape rect({(float)body.getSize().x, (float)body.getSize().y});
  rect.setOrigin(0.5 * body.getSize().x, 0.5 * body.getSize().y);
  rect.setPosition({(float)body.getPosition().x, (float)body.getPosition().y});
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

void SimulationSFML::drawArc(sf::RenderTarget &target, Vector_d position,
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

void SimulationSFML::drawPlayer(sf::RenderTarget &target,
                                const std::string &name,
                                const Robot &robot) const {
  // drawRobot
  drawRobot(target, robot);

  // drawLable
  drawLable(target, name, robot.getPosition());

  drawArc(target, robot.getPosition(),
          robot.getRotation() + robot.getTurretAngle());
}

void SimulationSFML::drawUI(sf::RenderTarget &target) const {
  sf::View old_view = target.getView();
  target.setView(sf::View({0.f, 0.f, static_cast<float>(target.getSize().x),
                           static_cast<float>(target.getSize().y)}));

  target.draw(fps_counter);
  // Other UI elements go here

  target.setView(old_view);
}

void SimulationSFML::drawLable(sf::RenderTarget &target,
                               const std::string &name,
                               const Vector_d &position) const {
  sf::Text name_tag(name, font, 15);
  name_tag.setOrigin(0.5 * name_tag.getLocalBounds().width, 0);
  name_tag.setPosition({(float)position.x, (float)position.y + 15});

  target.draw(name_tag);
}

void SimulationSFML::drawArena(sf::RenderTarget &target) const {
  sf::RectangleShape rect(
      {(float)rules.arena_size.x, (float)rules.arena_size.y});
  rect.setFillColor(sf::Color::Transparent);
  rect.setOutlineColor(sf::Color::White);
  rect.setOutlineThickness(5);
  target.draw(rect);
}
