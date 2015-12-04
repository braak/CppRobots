/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Angles.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __ANGLES__
#define __ANGLES__

// we need a custom modulo function, because fmod returns a value with the same
// sign as the dividend

/**
  Coustom modulus function.
  \param a The dividend
  \param n The divisor
  \return remainder as calculated by \f$ r = a - n
  \left\lfloor\frac{a}{n}\right\rfloor \f$
*/
template <class T> constexpr T modulus(T a, T n) {
  return a - floor(a / n) * n;
}

/**
  Wraps the angle from \f$ 2\pi\f$ to \f$ 0 \f$.
  \param angle The angle in radians
*/
template <class T> constexpr T wrapRadians(T angle) {
  return modulus<T>(angle, 2 * M_PI);
}

/**
  Calculate the smallest difference between two radiant angles
  \param angle The angle in radians
  \param b The second angle in radians
*/
template <class T> constexpr T angDiffRadians(T angle, T b = 0) {
  return modulus<T>(angle - b + M_PI, 2 * M_PI) - M_PI;
}

/**
  Wraps the angle from \f$ 360°\f$ to \f$ 0° \f$.
  \param angle The angle in degrees
*/
template <class T> constexpr T wrapDegrees(T angle) {
  return modulus<T>(angle, 360);
}

/**
  Calculate the smallest difference between two degree angles
  \param angle The angle in degrees
  \param b The second angle in degrees
*/
template <class T> constexpr T angDiffDegrees(T angle, T b = 0) {
  return modulus<T>(angle - b + 180, 360) - 180;
}

/**
  Convert an angle from degrees to randians.
*/
template <class T> constexpr T radians(T degrees) {
  return (degrees * M_PI) / 180;
}

/**
  Convert an angle from radians to degrees.
*/
template <class T> constexpr T degrees(T radians) {
  return (radians * 180) / M_PI;
}

#endif /* end of include guard: __ANGLES__ */
