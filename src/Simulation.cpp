/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Simulation.cpp
*   \author Jan-Niklas Braak
*/

#include "Simulation.hpp"

Simulation::Simulation(const Rules &rules_, unsigned int seed)
    : rules(rules_), generator(seed) {}

void Simulation::update() {
  updateProjectiles();
  updatePlayers();

  // remove players that don't have health left.
  auto pred = [](const std::pair<const std::string, Robot> &robot) {
    return robot.second.getHealth() <= 0;
  };
  auto it = players.begin();
  while ((it = std::find_if(it, players.end(), pred)) != players.end()) {
    std::string name = it->first;
    players.erase(it++);
    deathSignal(name);
  }

  simulationStepSignal();
  runTime += rules.timeStep;
}

void Simulation::updatePlayers() {
  /*NOTE: The Simulation behaves as if events during a turn happen simultaneous.
   * That way the simulation is deterministic, even though (partial) events may
   * occur in an arbitrary order. This is archived by applying types of event in
   * a particular order. E.g. player movements never influence each other.
   * Collisions never influence each other. But movement influences Collision.
   * Therefor if movement and collision were interleaved, the order of events
   * would matter. By splitting movement and collision, the order of events does
   * not matter.
   * If an event can influence events of its type, this system won’t work. One
   * solution to this problem is to split the event in parts, that influence
   * each other but not themselves.
   */

  // set vision for all players
  for (auto &player : players) {
    check_scan(player.second);
  }

  // Player Actions(Movement)
  for (auto &player : players) {
    player.second.update();
  }

  // resolve Player Actions(Shooting)
  for (auto &player : players) {
    // see if any player wants to shoot
    if (player.second.shooting) {
      // reset the shooting flag
      player.second.shooting = false;
      // calculate the direction in which the Robot shoots
      double direction =
          player.second.getRotation() + player.second.getTurretAngle();
      Vector_d porjectilePosition = player.second.getPosition();
      // make sure we create the Projectile outside the player
      porjectilePosition += Vector_d::polar(
          direction,
          Vector_d(rules.robot_size.x, rules.projectile_size.x).magnitude());
      // create the Projectile
      projectiles.push_back(
          Projectile(rules, porjectilePosition, direction, player.first));
    }
  }

  // resolve Player collision
  for (auto &player : players) {
    // check collision between playeres
    // NOTE: currently we check each pair of players twice, once for
    // Collision(A,B) and once for Collision(B,A).
    for (auto const &player2 : players) {
      if (&player == &player2) {
        // don't check collision with self.
        continue;
      }
      if (Collision(player.second, player2.second)) {
        collisionSignal(player.first, player2.first);
        player.second.takeDamage(rules.collision_damage);
      }
    }
  }

  // resolve out-of-Bound events
  for (auto &player : players) {
    // check if any player is outside the arena
    Vector_d pos = player.second.getPosition();
    if (pos.x > rules.arena_size.x || pos.y > rules.arena_size.y || pos.x < 0 ||
        pos.y < 0) {
      outOfBoundsSignal(player.first);
      player.second.takeDamage(rules.collision_damage);
    }
  }

  // resolve collision between players and projectiles
  for (auto &player : players) {
    // check collision between player and projectile
    for (auto projectile = projectiles.begin();
         projectile != projectiles.end();) {
      Collision collision(player.second, *projectile);
      if (collision) {
        // deal damage to the player
        player.second.takeDamage(rules.projectile_damage);
        hitSignal(player.first, projectile->owner);
        // remove projectile, and advance the iterator
        projectile = projectiles.erase(projectile);
      } else {
        // advance the iterator
        ++projectile;
      }
    }
  }
}

void Simulation::updateProjectiles() {
  // move the projectile
  for (auto &projectle : projectiles) {
    projectle.update();
  }
  // remove projectile if it is out of bounds
  projectiles.remove_if([this](const Projectile &projectle) {
    Vector_d pos = projectle.getBody().getPosition();
    return pos.x > rules.arena_size.x || pos.y > rules.arena_size.y ||
           pos.x < 0 || pos.y < 0;
  });
}

std::string Simulation::runtimeString() const {
  std::stringstream rt;

  // int days = runTimeInt / 60 / 60 / 24;
  // int hours = int(runTime / 60 / 60) % 24;
  int minutes = int(runTime / 60);
  float seconds = fmod(runTime, 60);

  rt << minutes << ":";
  rt << std::setfill('0') << std::setw(6) << std::fixed << std::setprecision(3)
     << seconds;
  return rt.str();
}

void Simulation::newPlayer(std::string name, Agent *agent) {

  const double safety = 0.1;
  std::uniform_real_distribution<double> x(rules.arena_size.x * safety,
                                           rules.arena_size.x * (1 - safety));
  std::uniform_real_distribution<double> y(rules.arena_size.y * safety,
                                           rules.arena_size.y * (1 - safety));
  std::uniform_real_distribution<double> rot(0, 2 * M_PI);

  newPlayer(name, agent, {x(generator), y(generator)}, rot(generator));
}

void Simulation::newPlayer(std::string name, Agent *agent, Vector_d position,
                           double rotation) {

  Robot player(rules, std::move(agent));
  player.setPosition(position);
  player.setRotation(rotation);
  players.insert({name, std::move(player)});
  newPlayerSignal(name);
  /*NOTE: maybe only spawn players at a specific time in the simulation cycle.
   * e.g. insert new player in newPlayers map and merge this map with players at
   * the end or start of the simulation cycle. That prevents wierdness and race
   * conditions.
   */
}

double Simulation::getRuntime() const { return runTime; }
int Simulation::getNumPlayers() const { return players.size(); }
bool Simulation::isRunning() const { return true; }

const std::map<std::string, Robot> &Simulation::getPlayers() const {
  return players;
}

const std::list<Projectile> &Simulation::getProjectiles() const {
  return projectiles;
}

bool Simulation::inScanArea(Vector_d const &p1, double rotation,
                            Vector_d const &p2) const {
  // Vector form robot to the target.
  const auto v = p2 - p1;

  if (v.magnitude() < rules.scan_proximity) {
    // targets are always visible when they are in scan_proximity range.
    return true;
  } else if (v.magnitude() > rules.scan_range) {
    // targets are never visible if they are out of scan_range.
    return false;
  }

  // If the Target is visible depends if it is within the Arc of Vision.
  const auto angle = angDiffRadians(rotation, v.angle());
  return -rules.scan_angle / 2 < angle && angle < rules.scan_angle / 2;
}

void Simulation::check_scan(Robot &robot) {
  std::list<std::shared_ptr<Robot>> scanTargets;
  for (auto const &player2 : players) {
    if (&robot == &player2.second) {
      continue;
    }
    const Vector_d pose1 = robot.getPosition();
    const double rotation =
        wrapRadians(robot.getRotation() + robot.getTurretAngle());
    const Vector_d pose2 = player2.second.getPosition();
    if (inScanArea(pose1, rotation, pose2)) {
      scanTargets.push_back(std::make_shared<Robot>(player2.second));
    }
  }
  robot.setScanTargets(std::move(scanTargets));
}
