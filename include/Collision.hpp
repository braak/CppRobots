/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Collision.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __COLLISION__
#define __COLLISION__

#include "Rectangle.hpp"
#include "Vector.hpp"
#include <vector>

class Collision {
  bool collision;
  // Vector_d MTV;

  struct Projection {
    double min;
    double max;
    constexpr bool overlap(const Projection &proj2) {
      return !(proj2.min > max || min > proj2.max);
    }
  };

  // Rectangle specific helper methode for projection
  static Projection project(const Rectangle &rect, const Vector_d &axis) {
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

  // Rectangle specific helper methode for calculating axes
  static std::vector<Vector_d> getAxes(const Rectangle &rect) {
    std::vector<Vector_d> axes(2);
    // the first axis is a unit vector in the direction of the Rectangle
    axes[0] = Vector_d::polar(rect.getRotation());
    // the second axis is perpendicular to the first
    axes[1] = {-axes[0].y, axes[0].x};
    // the other two axes are parallel to the first two
    return axes;
  }

public:
  Collision() : collision(false) {}

  // Collision(const Circle &circ1, const Circle &circ2);
  // Collision(const Rectangle &rect, const Circle &circ);
  // Collision(const Circle &circ, const Rectangle &rect);

  Collision(const Rectangle &rect1, const Rectangle &rect2) {
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
        // if the Projections dont overlap we have a seperating axis, the
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
        // if the Projections dont overlap we have a seperating axis, the
        // Rectangles don't collide.
        collision = false;
        return;
      }
    }
    // No seperating axis was found, the Rectangles collide.
  };

  template <class T, class U>
  Collision(const T &val1, const U &val2)
      : Collision(val1.getBody(), val2.getBody()) {}

  explicit operator bool() const { return collision; }

  static bool testCollision(const Rectangle &rect1, const Rectangle &rect2) {
    return static_cast<bool>(Collision(rect1, rect2));
  }
};

#endif /* end of include guard: __COLLISION__ */
