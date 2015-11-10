/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Robot.cpp
*   \author Jan-Niklas Braak
*/

#include "Robot.hpp"
#include <sstream>

Robot::Robot(const Rules &rules, Agent *agent_)
    : shooting(false), rules(rules), body(rules.robot_size),
      health(rules.max_health),
      agent(std::shared_ptr<Agent>(std::move(agent_))) {}

void Robot::setPosition(Vector_d position) { body.setPosition(position); }
Vector_d Robot::getPosition() const { return body.getPosition(); }

double Robot::getRotation() const { return body.getRotation(); }
void Robot::setRotation(double rotation) { body.setRotation(rotation); }

const Rectangle &Robot::getBody() const { return body; }

void Robot::update() {
  if (!agent) {
    throw std::runtime_error("No Agent was set for this Robot.");
  }
  Action a = agent->update(*this);

  const double v = std::max(std::min(a.v, rules.v_max), rules.v_min);
  const double w = std::max(std::min(a.w, rules.w_max), -rules.w_max);

  // move forward in the current direction
  body.move(Vector_d::polar(body.getRotation(), v * rules.timeStep));
  // turn
  body.rotate(w * rules.timeStep);

  // set the turretAngle
  // find the smallest angle between the current and the target angle
  double turnRate = angDiffRadians(a.turretAngle, turretAngle) / rules.timeStep;
  if (turnRate > rules.turret_w_max) {
    turretAngle += rules.turret_w_max * rules.timeStep;
  } else if (turnRate < -rules.turret_w_max) {
    turretAngle -= rules.turret_w_max * rules.timeStep;
  } else {
    turretAngle = a.turretAngle;
  }

  // shoot
  cooldown -= rules.timeStep;
  if (cooldown < 0 && a.shooting) {
    cooldown = rules.projectile_cooldown;
    shooting = true;
  }
}

void Robot::setAgent(Agent *agent_) { agent = std::shared_ptr<Agent>(agent_); }

void Robot::setScanTargets(std::list<std::shared_ptr<Robot>> scanTargets_) {
  scanTargets = scanTargets_;
}

std::list<std::shared_ptr<Robot>> Robot::scanAll() const { return scanTargets; }

std::shared_ptr<Robot> Robot::scanClosest() const {
  if (scanTargets.empty()) {
    return nullptr;
  }
  std::shared_ptr<Robot> target = scanTargets.front();
  for (const auto &currentTarget : scanTargets) {
    const Vector_d vector_a = target->getPosition() - body.getPosition();
    const Vector_d vector_b = currentTarget->getPosition() - body.getPosition();
    if (vector_a.magnitude() > vector_b.magnitude()) {
      target = currentTarget;
    }
  }
  return target;
}

std::shared_ptr<Robot> Robot::scanAny() const {
  if (scanTargets.empty()) {
    return nullptr;
  }
  return scanTargets.front();
}

void Robot::onCollision() { takeDamage(rules.collision_damage); }

double Robot::getHealth() const { return health; }
double Robot::takeDamage(double damage) {
  health -= damage;
  return health;
}

double Robot::getTurretAngle() const { return turretAngle; }

double Robot::limitRate(double oldVal, double newVal, double maxRate,
                        double minRate) {
  double turnRate = (newVal - oldVal) / rules.timeStep;
  if (turnRate > maxRate) {
    return oldVal + maxRate * rules.timeStep;
  } else if (turnRate < minRate) {
    return oldVal + minRate * rules.timeStep;
  } else {
    return newVal;
  }
}
std::ostream &operator<<(std::ostream &os, const Robot &obj) {
  os << "<Robot at " << obj.body.getPosition() << ">";
  return os;
}
