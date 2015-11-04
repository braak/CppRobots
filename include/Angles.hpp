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
template <class T> constexpr T modulus(T a, T n) {
  return a - floor(a / n) * n;
}

template <class T> constexpr T wrapRadians(T a) {
  return modulus<T>(a, 2 * M_PI);
}

template <class T> constexpr T angDiffRadians(T a, T b = 0) {
  return modulus<T>(a - b + M_PI, 2 * M_PI) - M_PI;
}

template <class T> constexpr T wrapDegrees(T a) { return modulus<T>(a, 360); }

template <class T> constexpr T angDiffDegrees(T a, T b = 0) {
  return modulus<T>(a - b + 180, 360) - 180;
}

template <class T> constexpr T radians(T degrees) {
  return (degrees * M_PI) / 180;
}

template <class T> constexpr T degrees(T radians) {
  return (radians * 180) / M_PI;
}

#endif /* end of include guard: __ANGLES__ */
