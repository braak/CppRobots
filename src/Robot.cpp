/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Robot.cpp
*   \author Jan-Niklas Braak
*/

#include "Robot.hpp"
#include <sstream>

Robot::Robot(const double timeStep, const Rules &rules)
    : rules(rules), body(rules.robot_size), health(rules.max_health),
      timeStep(timeStep) {}

void Robot::setPosition(Vector_d position) { body.setPosition(position); }
Vector_d Robot::getPosition() const { return body.getPosition(); }

double Robot::getRotation() const { return body.getRotation(); }
void Robot::setRotation(double rotation) { body.setRotation(rotation); }

const Rectangle &Robot::getBody() const { return body; }

void Robot::update(Action const &a) {
  const double v = std::max(std::min(a.v, rules.v_max), rules.v_min);
  const double w = std::max(std::min(a.w, rules.w_max), -rules.w_max);

  // move forward in the current direction
  body.move(Vector_d::polar(body.getRotation(), v * timeStep));
  // turn
  body.rotate(w * timeStep);
}

void Robot::setScanTargets(std::list<std::shared_ptr<Robot>> scanTargets_) {
  scanTargets = scanTargets_;
}

std::list<std::shared_ptr<Robot>> Robot::getScanTargets() const {
  return scanTargets;
}

void Robot::onCollision() { health -= rules.collision_damage; }

double Robot::getHealth() const { return health; }

std::ostream &operator<<(std::ostream &os, const Robot &obj) {
  os << "<Robot at " << obj.body.getPosition() << ">";
  return os;
}
