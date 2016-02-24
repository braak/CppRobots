/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
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
  // NOTE: currently this does not need to be its own class. But with the
  // introduction of a MTV it is more usefull to have the result of a collision
  // test in one class.
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

    controll structures in C++ will try to explicitly cast a type to see if it
    is true. so the following sniped will be valid:

    \code{.cpp}
      if(Collision(r1, r2)){
        // do stuff
      }
    \endcode
    However assignment to a boolean variable or use in a function that expects
    a boolean requires explicit conversion:
    \code{.cpp}
      auto collision = Collision(r1, r2);
      bool hit = (bool) collision;
      foo((bool) collision);
    \endcode
  */
  explicit operator bool() const;
};

#endif /* end of include guard: __COLLISION__ */
