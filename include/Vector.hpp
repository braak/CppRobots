/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Vector.hpp
*   \author Jan-Niklas Braak
*/

/** \addtogroup math
 *  @{
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
  constexpr Vector() : x(0), y(0){};

  /**
  Constructor.
  \param x x coordinate of the Vector.
  \param y y coordinate of the Vector
  */
  constexpr Vector(T x, T y) : x(x), y(y){};

  /**
    Constructor.

    Constructs a Vector from any other type of Vector.
    \param vector any type of vector.
  */
  template <typename U>
  explicit constexpr Vector(const Vector<U> &vector)
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
  \return the inverse of the Vector.
  */
  constexpr Vector<T> operator-() const { return Vector(-x, -y); }

  /**
  Rotates the Vector by the given rotation.
  \param rotation the amount to rotate tne Vector by.
  \return *this for method chaining.
  */
  Vector<T> &rotate(double rotation) {
    const double x_ = x * cos(rotation) - y * sin(rotation);
    y = x * sin(rotation) + y * cos(rotation);
    x = x_;

    return *this;
  }

  /**
    The magnitude of the Vector.
    \return the magnitude of the Vector.
  */
  constexpr T magnitude() const { return sqrt(pow(x, 2) + pow(y, 2)); }

  /**
    The angle of the Vector.
    \return the angle of the Vector.
  */
  constexpr T angle() const { return atan2(y, x); }

  /**
  Dot product of two Vectors.
  \param rhs right hand side of the operation.
  \return the scalar value.
  */
  constexpr T dot(const Vector<T> &rhs) const { return x * rhs.x + y * rhs.y; }

  /**
  The zero vector, \f$\vec{0}\f$.
  \return the new Vector.
  */
  constexpr static Vector<T> zero() { return {0, 0}; }
  /**
  The one vector, \f$\vec{1}\f$.
  \return the new Vector.
  */
  constexpr static Vector<T> one() { return {1, 1}; }
  /**
  The unit vector in x direction, \f$\vec{e}_x\f$.
  \return the new Vector.
  */
  constexpr static Vector<T> unit_x() { return {1, 0}; }
  /**
  The unit vector in y direction, \f$\vec{e}_y\f$.
  \return the new Vector.
  */
  constexpr static Vector<T> unit_y() { return {0, 1}; }

  /**
  Constructs a polar Vector.
  \param angle the angle of the Vector.
  \param magnitude the magnitide of the Vector.
  \return the new Vector.
  */
  constexpr static Vector<T> polar(double angle, double magnitude = 1) {
    return {static_cast<T>(magnitude * cos(angle)),
            static_cast<T>(magnitude * sin(angle))};
  }

  // Member data

  T x; //!< the x value of the Vector.
  T y; //!< the y value of the Vector.
};

/**
  Binary addition of two Vectors.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return the new Vector.
*/
template <typename T>
constexpr Vector<T> operator+(Vector<T> lhs, const Vector<T> &rhs) {
  return lhs += rhs;
}
/**
  Binary subtraction of two Vectors.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return the new Vector.
*/
template <typename T>
constexpr Vector<T> operator-(Vector<T> lhs, const Vector<T> &rhs) {
  return lhs -= rhs;
}
/**
  Binary multiplication of a Vector and a scalar.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return the new Vector.
*/
template <typename T> constexpr Vector<T> operator*(Vector<T> lhs, T rhs) {
  return lhs *= rhs;
}

/**
  Binary multiplication of a scalar and a Vector.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return the new Vector.
*/
template <typename T> constexpr Vector<T> operator*(T lhs, Vector<T> rhs) {
  return rhs *= lhs;
}

/**
  Binary division of a Vector and a scalar.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return the new Vector.
*/
template <typename T> constexpr Vector<T> operator/(Vector<T> lhs, T rhs) {
  return lhs /= rhs;
}

/**
Dot product of two Vectors.
\param lhs left hand side of the operation.
\param rhs right hand side of the operation.
\return the scalar result.
*/
template <typename T>
constexpr T dot(const Vector<T> &lhs, const Vector<T> &rhs) {
  return lhs.dot(rhs);
}

/**
  Equality test.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return true when the Vectors are equal, false otherwise.
*/
template <typename T>
constexpr bool operator==(const Vector<T> &lhs, const Vector<T> &rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}
/**
  Inequality test.
  \param lhs left hand side of the operation.
  \param rhs right hand side of the operation.
  \return true when the Vectors are inequal, false otherwise.
*/
template <typename T>
constexpr bool operator!=(const Vector<T> &lhs, const Vector<T> &rhs) {
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
  os << "(" << obj.x << ", " << obj.y << ")";
  return os;
}

// common types
using Vector_i = Vector<int>; //!< Vector specialization for int
using Vector_u =
    Vector<unsigned int>;        //!< Vector specialization for unsigned int
using Vector_f = Vector<float>;  //!< Vector specialization for float
using Vector_d = Vector<double>; //!< Vector specialization for double

#endif /* end of include guard: __VECTOR__ */

/** @}*/
