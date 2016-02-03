/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Angles.hpp
*   \author Jan-Niklas Braak
*/

/** \addtogroup math
 *  @{
 */

#ifndef __MATH_ANGLES__
#define __MATH_ANGLES__

#include "mathUtility.hpp"

/**
  Wraps the angle from \f$ 2\pi\f$ to \f$ 0 \f$.
  \param angle The angle in radians
*/
template <class T> constexpr T wrapRadians(T angle) {
  return modulus(angle, 2 * M_PI);
}

/**
  Calculate the smallest difference between two radiant angles.
  \param angle The angle in radians
  \param b The second angle in radians
*/
template <class T> constexpr T angDiffRadians(T angle, T b = 0) {
  return modulus(angle - b + M_PI, 2 * M_PI) - M_PI;
}

/**
  Wraps the angle from \f$ 360°\f$ to \f$ 0° \f$.
  \param angle The angle in degrees
*/
template <class T> constexpr T wrapDegrees(T angle) {
  return modulus(angle, 360);
}

/**
  Calculate the smallest difference between two degree angles.
  \param angle The angle in degrees
  \param b The second angle in degrees
*/
template <class T> constexpr T angDiffDegrees(T angle, T b = 0) {
  return modulus(angle - b + 180, 360) - 180;
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

#endif /* end of include guard: __MATH_ANGLES__ */

/** @}*/
