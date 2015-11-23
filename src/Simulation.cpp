/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Simulation.cpp
*   \author Jan-Niklas Braak
*/

#include "Simulation.hpp"

Simulation::Simulation(const Rules &rules_, unsigned int seed)
    : rules(rules_), generator(seed) {}

void Simulation::updatePlayers() {
  // set vision for all players
  for (auto &player : players) {
    check_scan(player.second);
  }
  // Player Actions
  for (auto &player : players) {
    player.second.update();
  }

  // resolve Player Actions
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

    // check collision between playeres
    // std::list<Robot *> collisions;
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

    // check if any player is outside the arena
    Vector_d pos = player.second.getPosition();
    if (pos.x > rules.arena_size.x || pos.y > rules.arena_size.y || pos.x < 0 ||
        pos.y < 0) {
      outOfBoundsSignal(player.first);
      player.second.takeDamage(rules.collision_damage);
    }

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

  runTime += rules.timeStep;
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

bool Simulation::inScanArea(Vector_d const &p1, double rotation,
                            Vector_d const &p2) const {
  const Vector_d v = p2 - p1;

  // const bool in_range = v.magnitude() < rules.scan_range;
  if (v.magnitude() < rules.scan_proximity) {
    return true;
  } else if (v.magnitude() > rules.scan_range) {
    // early out, we don't have to check the angles.
    return false;
  }

  const double beta = angDiffRadians(rotation, v.angle());
  const bool in_segment =
      -rules.scan_angle / 2 < beta && beta < rules.scan_angle / 2;

  return in_segment;
}

void Simulation::check_scan(Robot &robot) {
  std::list<std::shared_ptr<Robot>> scanTargets;
  for (auto const &player2 : players) {
    if (&robot == &player2.second) {
      continue;
    }
    Vector_d pose1 = robot.getPosition();
    double rotation = wrapRadians(robot.getRotation() + robot.getTurretAngle());
    Vector_d pose2 = player2.second.getPosition();
    if (inScanArea(pose1, rotation, pose2)) {
      scanTargets.push_back(std::make_shared<Robot>(player2.second));
    }
  }
  robot.setScanTargets(std::move(scanTargets));
}
