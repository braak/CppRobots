/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file ViewSFML.cpp
*   \author Jan-Niklas Braak
*/

#include "ViewSFML.hpp"

ViewSFML::ViewSFML() : ViewSFML(nullptr) {}

ViewSFML::ViewSFML(Simulation *sim)
    : window(sf::VideoMode::getDesktopMode(), window_name), logging(logLength) {

  setSimulation(std::shared_ptr<Simulation>(sim));
  // Load resources
  if (!font.loadFromFile(selfpath() + "/" + fontName)) {
    throw std::runtime_error("unable to load font " + fontName);
  }
}

void ViewSFML::setSimulation(std::shared_ptr<const Simulation> sim) {
  simulation = sim;

  if (!simulation) {
    return;
  }

  const Rules &rules = simulation->rules;

  sf::View view = window.getView();
  view.setCenter(
      {(float)rules.arena_size.x / 2, (float)rules.arena_size.y / 2});
  window.setView(view);

  frameTimer.setTimeStep(std::chrono::duration<double>(rules.timeStep));
}

void ViewSFML::input() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }
    if (event.type == sf::Event::Resized) {
      // The Window was resized, resize the window.view apropriatly.
      sf::View new_view = window.getView();
      new_view.setSize(static_cast<sf::Vector2f>(window.getSize()));
      new_view.zoom(zoom_level);
      window.setView(new_view);
    }
    if (event.type == sf::Event::MouseWheelMoved) {
      // Mouswheel was moved, change the zoom level.
      sf::View new_view = window.getView();
      const double zoom = pow(zoom_speed, event.mouseWheel.delta);
      new_view.zoom(zoom); // zoom the view
      zoom_level *= zoom;  // save the current zoom_level (needed for resizing)
      window.setView(new_view);
    }
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Key::Escape) {
        window.close();
      }
      if (event.key.code == sf::Keyboard::Key::Space) {
        // Take schreenshot
        static int num = 0;
        window.capture().saveToFile("screenshot" + std::to_string(num++) +
                                    ".png");
      }
    }
  }
}

void ViewSFML::output() {
  if (!simulation) {
    throw std::runtime_error("No Simulation was set for this View.");
  }

  frameTimer.sync();

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

void ViewSFML::finish() {
  // if (!simulation) {
  //   throw std::runtime_error("No Simulation was set for this View.");
  // }
  // const Rules &rules = simulation->rules;

  while (isRunning()) {
    input();

    window.clear(sf::Color::Black);

    // switch View to Screen space
    sf::View old_view = window.getView();
    sf::Vector2f size = (sf::Vector2f)window.getSize();
    window.setView(sf::View({0.f, 0.f, size.x, size.y}));

    // Draw Text in center of schreen
    sf::Text game_over_text("Game Over ", font, 32);
    game_over_text.setPosition({size.x / 2, size.y / 2});
    game_over_text.setOrigin(0.5 * game_over_text.getLocalBounds().width, 0);
    window.draw(game_over_text);

    // switch back
    window.setView(old_view);

    // draw old UI on top of it.
    drawUI(window);

    window.display();
  }
}
void ViewSFML::log(std::string text) {
  if (!simulation) {
    throw std::runtime_error("No Simulation was set for this View.");
  }
  logging[logIndex++ % logLength] = simulation->runtimeString() + ": " + text;
}

bool ViewSFML::isRunning() const { return window.isOpen(); }

void ViewSFML::drawProjectile(sf::RenderTarget &target,
                              const Projectile &projectile) const {
  Rectangle body = projectile.getBody();

  sf::RectangleShape rect({(float)body.getSize().x, (float)body.getSize().y});
  rect.setOrigin(0.5 * body.getSize().x, 0.5 * body.getSize().y);
  rect.setPosition({(float)body.getPosition().x, (float)body.getPosition().y});
  rect.setRotation(degrees(body.getRotation()));
  target.draw(rect);
}
void ViewSFML::drawPlayer(sf::RenderTarget &target, const std::string &name,
                          const Robot &robot) const {
  // drawRobot
  drawRobot(target, robot);

  // drawLable
  drawLable(target, name, robot.getPosition());

  // drawArc(target, robot.getPosition(),
  //         robot.getRotation() + robot.getTurretAngle());
}
void ViewSFML::drawArc(sf::RenderTarget &target, Vector_d position,
                       double rotation) const {
  if (!simulation) {
    throw std::runtime_error("No Simulation was set for this View.");
  }
  const Rules &rules = simulation->rules;

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
void ViewSFML::drawLable(sf::RenderTarget &target, const std::string &name,
                         const Vector_d &position) const {
  sf::Text name_tag(sf::String::fromUtf8(name.begin(), name.end()), font, 15);
  name_tag.setOrigin(0.5 * name_tag.getLocalBounds().width, 0);
  name_tag.setPosition({(float)position.x, (float)position.y + 15});

  target.draw(name_tag);
}
void ViewSFML::drawRobot(sf::RenderTarget &target, const Robot &robot) const {
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

void ViewSFML::drawUI(sf::RenderTarget &target) const {
  // Schich View to Screenspace
  sf::View old_view = target.getView();
  sf::Vector2f size = (sf::Vector2f)target.getSize();
  target.setView(sf::View({0.f, 0.f, size.x, size.y}));

  // Draw fps counter
  sf::Text fps_counter(frameTimer.getOutput(), font, 12);
  target.draw(fps_counter);

  // Draw Log
  const int spacing = 14;
  sf::Text log_line("", font, 12);
  for (int i = 0; i < logLength; i++) {
    const std::string &line = logging[(logIndex + i) % logLength];
    if (!line.empty()) {
      log_line.setString(sf::String::fromUtf8(line.begin(), line.end()));
      log_line.move({0, spacing});

      target.draw(log_line);
    }
  }

  // restore view
  target.setView(old_view);
}
void ViewSFML::drawArena(sf::RenderTarget &target) const {
  if (!simulation) {
    throw std::runtime_error("No Simulation was set for this View.");
  }
  const Rules &rules = simulation->rules;
  sf::RectangleShape rect(
      {(float)rules.arena_size.x, (float)rules.arena_size.y});
  rect.setFillColor(sf::Color::Transparent);
  rect.setOutlineColor(sf::Color::White);
  rect.setOutlineThickness(5);
  target.draw(rect);
}
