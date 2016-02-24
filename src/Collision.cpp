/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Collision.cpp
*   \author Jan-Niklas Braak
*/

#include "Collision.hpp"

bool Collision::Projection::overlap(const Projection &proj2) {
  /*
    There are two cases where the projections don't overlarp:
      1. The first projection is completely to the left of the second
      2. The first projection is completely to the right of the second
    If neither is true the Projections overlap. In that way projections are
    similar to ranges.
  */
  return !(proj2.min > max || min > proj2.max);
}

Collision::Collision() : collision(false) {}

Collision::Projection Collision::project(const Rectangle &rect,
                                         const Vector_d &axis) {
  auto vertices = rect.vertices();
  double min = axis.dot(vertices[0]);
  double max = min;

  for (const auto &vertex : vertices) {
    // The projection of a vertix on an axis is the dot product of the two.
    double p = axis.dot(vertex);
    // remember the minimal and maximal projection.
    if (p < min) {
      min = p;
    } else if (p > max) {
      max = p;
    }
  }
  return {min, max};
}

std::vector<Vector_d> Collision::getAxes(const Rectangle &rect) {
  // the first axis is a unit vector in the direction of the Rectangle
  const auto axis0 = Vector_d::polar(rect.getRotation());
  // the second axis is perpendicular to the first
  const auto axis1 = axis0.perp();
  // the other two axes are parallel to the first two, so we don't need them.
  return {axis0, axis1};
}

Collision::Collision(const Rectangle &rect1, const Rectangle &rect2) {
  // NOTE: we could calculate the MTV by finding the axis with the bigest
  // overlap, and the coresponding overlap amount.

  /* NOTE: For other convex polygonal shapes this function is identical if
   * getAxes() and project() is defined for them. This could be unified by
   * having a   "Collider" parent class. Collision with a circle is a special
   * case. collision   of two circles is trivial. Concave polygons have to be
   * seperated into convex shapes.
   */
  collision = true;
  const auto axes1 = getAxes(rect1);
  const auto axes2 = getAxes(rect2);

  /* This collision test uses the separating axis theorem (SAT). In short the
   * SAT says that if two _convex_ objects don't overlap there is a axis
   * seperating them. That means if we find an axis that seperates the
   * rectangles we have no collision. On the other hand we have to test all
   * possible axes to verify a  collision. Fortunatly the axes we only have to
   * test the axes that are perpendicular to one of the sides of the object.
   */
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
