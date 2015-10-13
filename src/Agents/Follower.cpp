/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Follower.cpp
*   \author Jan-Niklas Braak
*/

#include "Agents/Follower.hpp"

Follower::Follower(double target_distance, double K_distance, double K_beta)
    : target_distance(target_distance), K_distance(K_distance), K_beta(K_beta) {
}

Robot::Action Follower::update(Robot const &r) {
  auto scanTargets = r.getScanTargets();

  if (scanTargets.empty()) {
    // If no visible Robot, turn in circle
    return {0, -r.w_max};
  }
  Pose pose = r.getPose();
  /*
    find the closes visible Robot
  */
  auto cmp = [pose](const std::shared_ptr<Robot> &a,
                    const std::shared_ptr<Robot> &b) -> bool {
    Pose pose_a = a->getPose();
    Pose pose_b = b->getPose();
    double distance_a =
        sqrt(pow(pose.x - pose_a.x, 2) + pow(pose.y - pose_a.y, 2));
    double distance_b =
        sqrt(pow(pose.x - pose_b.x, 2) + pow(pose.y - pose_b.y, 2));
    return distance_a < distance_b;
  };
  scanTargets.sort(cmp);

  std::shared_ptr<Robot> target_robot = scanTargets.front();
  Pose target_pose = target_robot->getPose();

  const double v_x = pose.x - target_pose.x;
  const double v_y = pose.y - target_pose.y;
  double beta = angDiffRadians(pose.theta, atan2(v_y, v_x));
  double distance =
      sqrt(pow(pose.x - target_pose.x, 2) + pow(pose.y - target_pose.y, 2));
  return {K_distance * (distance - target_distance), K_beta * beta};
}
