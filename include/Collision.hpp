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

/**
  Tests a collision between two objects.
*/
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
  /**
    Default constructor.

    A default constructed Collision represents no Collision.
  */
  Collision();

  // Collision(const Circle &circ1, const Circle &circ2);
  // Collision(const Rectangle &rect, const Circle &circ);
  // Collision(const Circle &circ, const Rectangle &rect);

  /**
    Collisoion between two Rectangles.
  */
  Collision(const Rectangle &rect1, const Rectangle &rect2);

  /**
    Collision between two objects that have a body(i.e. they have a getBody
    function.)
  */
  template <class T, class U>
  Collision(const T &val1, const U &val2)
      : Collision(val1.getBody(), val2.getBody()) {}

  /**
    Test if there was a collision.
  */
  explicit operator bool() const;
};

#endif /* end of include guard: __COLLISION__ */
