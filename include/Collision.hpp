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
    bool overlap(const Projection &proj2);
  };

  // Rectangle specific helper methode for projection
  static Projection project(const Rectangle &rect, const Vector_d &axis);

  // Rectangle specific helper methode for calculating axes
  static std::vector<Vector_d> getAxes(const Rectangle &rect);

public:
  Collision();

  // Collision(const Circle &circ1, const Circle &circ2);
  // Collision(const Rectangle &rect, const Circle &circ);
  // Collision(const Circle &circ, const Rectangle &rect);

  Collision(const Rectangle &rect1, const Rectangle &rect2);

  template <class T, class U>
  Collision(const T &val1, const U &val2)
      : Collision(val1.getBody(), val2.getBody()) {}

  explicit operator bool() const;

  static bool testCollision(const Rectangle &rect1, const Rectangle &rect2);
};

#endif /* end of include guard: __COLLISION__ */
