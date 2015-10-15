/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Vector.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __VECTOR__
#define __VECTOR__

#include "math.h"
#include <iostream>

/**
  A two-dimensional Vector.
*/
template <typename T> class Vector {
public:
  /**
  Default Contructor.
  */
  Vector() : x(0), y(0){};

  /**
  Constructor.
  \param x x coordinate of the Vector.
  \param y y coordinate of the Vector
  */
  Vector(T x, T y) : x(x), y(y){};

  /**
    Constructor.

    Converts any type of Vector into any othe.
    \param vector any type of vector.
  */
  template <typename U>
  explicit Vector(const Vector<U> &vector)
      : x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y)) {}

  /**
    Inplace addition of Vectors.
    \param rhs right hand side of the operation.
    \return the new Vector.
  */
  Vector<T> &operator+=(const Vector<T> &rhs) {
    x += rhs.x;
    y += rhs.y;

    return *this;
  }
  /**
    Inplace subtraction of Vectors.
    \param rhs right hand side of the operation.
    \return the new Vector.
  */
  Vector<T> &operator-=(const Vector<T> &rhs) {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
  }
  /**
    Inplace scalar multiplication .
    \param rhs right hand side of the operation.
    \return the new Vector.
  */
  Vector<T> &operator*=(T rhs) {
    x *= rhs;
    y *= rhs;

    return *this;
  }
  /**
    Inplace scalar division .
    \param rhs right hand side of the operation.
    \return the new Vector.
  */
  Vector<T> &operator/=(T rhs) {
    x /= rhs;
    y /= rhs;

    return *this;
  }
  /**
  Unary Minus.
  \return the new Vector.
  */
  Vector<T> operator-() { return Vector(-x, -y); }

  /**
  Dotproduct of two Vectors.
  \param rhs right hand side of the operation.
  \return the scalar value.
  */
  T dot(Vector<T> &rhs) { return x * rhs.x + y * rhs.y; }

  /**
  The zero vector, \f$\vec{0}\f$.
  \return the new Vector.
  */
  static Vector<T> zero() { return {0, 0}; }
  /**
  The one vector, \f$\vec{1}\f$.
  \return the new Vector.
  */
  static Vector<T> one() { return {1, 1}; }
  /**
  The unit vector in x direction, \f$\vec{e}_x\f$.
  \return the new Vector.
  */
  static Vector<T> unit_x() { return {1, 0}; }
  /**
  The unit vector in y direction, \f$\vec{e}_y\f$.
  \return the new Vector.
  */
  static Vector<T> unit_y() { return {0, 1}; }

  /**
  Constructs a polar Vector.
  \param angle the angle of the Vector.
  \param magnitude the magnitide of the Vector.
  \return the new Vector.
  */
  static Vector<T> polar(double angle, double magnitude = 1) {
    return {static_cast<T>(magnitude * cos(angle)),
            static_cast<T>(magnitude * sin(angle))};
  }

  // Member data
  union {
    T x;     //!< the x value of the Vector. The same as width.
    T width; //!< the withd value of the Vector. The same as x.
  };
  union {
    T y;      //!< the y value of the Vector. The same as height.
    T height; //!< the height value of the Vector. The same as y.
  };
};

/**
  Binary addition of two Vectors.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return the new Vector.
*/
template <typename T> Vector<T> operator+(Vector<T> lhs, const Vector<T> &rhs) {
  lhs += rhs;
  return lhs;
}
/**
  Binary subtraction of two Vectors.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return the new Vector.
*/
template <typename T> Vector<T> operator-(Vector<T> lhs, const Vector<T> &rhs) {
  lhs -= rhs;
  return lhs;
}
/**
  Binary multiplication of a Vector and a scalar.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return the new Vector.
*/
template <typename T> Vector<T> operator*(Vector<T> lhs, T rhs) {
  lhs *= rhs;
  return lhs;
}

/**
  Binary multiplication of a scalar and a Vector.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return the new Vector.
*/
template <typename T> Vector<T> operator*(T lhs, Vector<T> rhs) {
  rhs *= lhs;
  return rhs;
}

/**
  Binary division of a Vector and a scalar.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return the new Vector.
*/
template <typename T> Vector<T> operator/(Vector<T> lhs, T rhs) {
  lhs /= rhs;
  return lhs;
}

/**
  Equality test.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return true when the Vectors are equal, false otherwise.
*/
template <typename T>
bool operator==(const Vector<T> &lhs, const Vector<T> &rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}
/**
  Inequality test.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return true when the Vectors are inequal, false otherwise.
*/
template <typename T>
bool operator!=(const Vector<T> &lhs, const Vector<T> &rhs) {
  return !(lhs == rhs);
}

/**
Output stream Operator.
\param os the output stream.
\param obj the Vector to output.
\return the output stream.
*/
template <typename T>
std::ostream &operator<<(std::ostream &os, const Vector<T> &obj) {
  os << "Vector(" << obj.x << obj.y << ")";
  return os;
}

// common types
using Vector_i = Vector<int>; //!< Vector specialization for int
using Vector_u =
    Vector<unsigned int>;       //!< Vector specialization for unsigned int
using Vector_f = Vector<float>; //!< Vector specialization for float

#endif /* end of include guard: __VECTOR__ */
