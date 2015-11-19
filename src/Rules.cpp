/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Rules.cpp
*   \author Jan-Niklas Braak
*/

#include "Rules.hpp"

std::istream &operator>>(std::istream &is, Rules &rules) {
  Rules defaultRules = Rules::defaultRules();
  Json::Value root;
  is >> root;

  rules.timeStep = root.get("timeStep", defaultRules.timeStep).asDouble();
  rules.scan_range = root.get("scan_range", defaultRules.scan_range).asDouble();
  rules.scan_proximity =
      root.get("scan_proximity", defaultRules.scan_proximity).asDouble();
  rules.scan_angle = root.get("scan_angle", defaultRules.scan_angle).asDouble();

  Json::Value rs = root["robot_size"];
  rules.robot_size.x = rs.get("x", defaultRules.robot_size.x).asDouble();
  rules.robot_size.y = rs.get("y", defaultRules.robot_size.y).asDouble();

  Json::Value as = root["arena_size"];
  rules.arena_size.x = as.get("x", defaultRules.arena_size.x).asDouble();
  rules.arena_size.y = as.get("y", defaultRules.arena_size.y).asDouble();

  rules.v_max = root.get("v_max", defaultRules.v_max).asDouble();
  rules.v_min = root.get("v_min", defaultRules.v_min).asDouble();
  rules.w_max = root.get("w_max", defaultRules.w_max).asDouble();
  rules.turret_w_max =
      root.get("turret_w_max", defaultRules.turret_w_max).asDouble();

  rules.collision_damage =
      root.get("collision_damage", defaultRules.collision_damage).asDouble();
  rules.max_health = root.get("max_health", defaultRules.max_health).asDouble();

  Json::Value ps = root["projectile_size"];
  rules.projectile_size.x =
      ps.get("x", defaultRules.projectile_size.x).asDouble();
  rules.projectile_size.y =
      ps.get("y", defaultRules.projectile_size.y).asDouble();

  rules.projectile_speed =
      root.get("projectile_speed", defaultRules.projectile_speed).asDouble();
  rules.projectile_cooldown =
      root.get("projectile_cooldown", defaultRules.projectile_cooldown)
          .asDouble();
  rules.projectile_damage =
      root.get("projectile_damage", defaultRules.projectile_damage).asDouble();

  return is;
}

std::ostream &operator<<(std::ostream &os, const Rules &rules) {
  Json::Value root;
  root["timeStep"] = rules.timeStep;

  root["scan_range"] = rules.scan_range;
  root["scan_proximity"] = rules.scan_proximity;
  root["scan_angle"] = rules.scan_angle;

  root["robot_size"]["x"] = rules.robot_size.x;
  root["robot_size"]["y"] = rules.robot_size.y;

  root["arena_size"]["x"] = rules.arena_size.x;
  root["arena_size"]["y"] = rules.arena_size.y;

  root["v_max"] = rules.v_max;
  root["v_min"] = rules.v_min;
  root["w_max"] = rules.w_max;

  root["collision_damage"] = rules.collision_damage;
  root["max_health"] = rules.max_health;

  root["projectile_size"]["x"] = rules.projectile_size.x;
  root["projectile_size"]["y"] = rules.projectile_size.y;

  root["projectile_speed"] = rules.projectile_speed;
  root["projectile_cooldown"] = rules.projectile_cooldown;
  root["projectile_damage"] = rules.projectile_damage;

  os << root;
  return os;
}
