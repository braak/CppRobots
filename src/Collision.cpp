/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Collision.cpp
*   \author Jan-Niklas Braak
*/

#include "Collision.hpp"

bool Collision::Projection::overlap(const Projection &proj2) {
  return !(proj2.min > max || min > proj2.max);
}

Collision::Collision() : collision(false) {}

Collision::Projection Collision::project(const Rectangle &rect,
                                         const Vector_d &axis) {
  auto vertices = rect.vertices();
  double min = axis.dot(vertices[0]);
  double max = min;

  for (const auto &vertex : vertices) {
    double p = axis.dot(vertex);
    if (p < min) {
      min = p;
    } else if (p > max) {
      max = p;
    }
  }
  return {min, max};
}

std::vector<Vector_d> Collision::getAxes(const Rectangle &rect) {
  std::vector<Vector_d> axes(2);
  // the first axis is a unit vector in the direction of the Rectangle
  axes[0] = Vector_d::polar(rect.getRotation());
  // the second axis is perpendicular to the first
  axes[1] = {-axes[0].y, axes[0].x};
  // the other two axes are parallel to the first two, so we don't need them.
  return axes;
}

Collision::Collision(const Rectangle &rect1, const Rectangle &rect2) {
  // NOTE: we could calculate the MTV by finding the axis with the bigest
  // overlap, and the coresponding overlap amount.
  collision = true;
  const auto axes1 = getAxes(rect1);
  const auto axes2 = getAxes(rect2);

  for (const auto &axis : axes1) {
    // Project both Rectangles onto the axis
    Projection p1 = project(rect1, axis);
    Projection p2 = project(rect2, axis);
    if (!p1.overlap(p2)) {
      // if the Projections don't overlap we have a seperating axis, the
      // Rectangles don't collide.
      collision = false;
      return;
    }
  }

  for (const auto &axis : axes2) {
    // Project both Rectangles onto the axis
    Projection p1 = project(rect1, axis);
    Projection p2 = project(rect2, axis);
    if (!p1.overlap(p2)) {
      // if the Projections don't overlap we have a seperating axis, the
      // Rectangles don't collide.
      collision = false;
      return;
    }
  }
  // No seperating axis was found, the Rectangles collide.
}

Collision::operator bool() const { return collision; }

bool Collision::testCollision(const Rectangle &rect1, const Rectangle &rect2) {
  return static_cast<bool>(Collision(rect1, rect2));
}
