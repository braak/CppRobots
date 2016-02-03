/*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file mathUtility.hpp
*   \author Jan-Niklas Braak
*/

/** @defgroup math Math
 * A collection of mathematics related classes and functions.
 *  @{
 */

#ifndef __MATH_UTILITY__
#define __MATH_UTILITY__

/**
  Coustom modulus function.

  we need a custom modulo function, for wrap, because fmod returns a value with
  the same sign as the dividend.
  \f$ modulus(a, n) = a - n \left\lfloor\frac{a}{n}\right\rfloor \f$
  \param a The dividend
  \param n The divisor
  \return the remainder.
*/
template <class T> constexpr T modulus(T a, T n) {
  return a - floor(a / n) * n;
}

/**
  Limits the range a value can take by wraping it around.
  \f$ wrap(value, lowerLimit, upperLimit) = value -
  \left\lfloor\frac{value - lowerLimit}{upperLimit - lowerLimit}\right\rfloor
  (upperLimit - lowerLimit)
  \f$

  \param value the value to clamp.
  \param lowerLimit The lower limit of the value.
  \param upperLimit the upper limit of the value.
  \return the wraped value.
*/
template <class T> constexpr T wrap(T value, T lowerLimit, T upperLimit) {
  return value -
         floor((value - lowerLimit) / (upperLimit - lowerLimit)) *
             (upperLimit - lowerLimit);
  // return modulus(value - lowerLimit, upperLimit - lowerLimit) + lowerLimit;
}

/**
  Limits the range a value can take by 'clamping' it.
  \f$ clamp(value, lowerLimit, upperLimit)= \begin{cases}
      upperLimit &  \text{ if } value \gt upperLimit \\
      lowerLimit &  \text{ if } value \lt lowerLimit \\
      value &  \text{ otherwise }
   \end{cases} \f$
  \param value the value to clamp.
  \param upperLimit the upper limit of the value.
  \param lowerLimit The lower limit of the value.
  \return the claped value.
*/
template <class T> constexpr T clamp(T value, T lowerLimit, T upperLimit) {
  /* NOTE: we are using conditional expressions(the ternary operator "?:") to
     allow clamp to be a constexpr. Using ordinary if-else statements the
     fuction body would look like this:

     if (value > upperLimit) {
       return upperLimit;
     } else if (value < lowerLimit) {
       return lowerLimit;
     }
     return value;
  */
  return (upperLimit < value) ? upperLimit
                              : ((value < lowerLimit) ? lowerLimit : value);
}

/**
  Linearly interpolate between two values.
  \param v0 the first value.
  \param v1 the second value.
  \param t interpolation amount. A value between 0 and 1.
  \return the interpolate value.-
*/
template <class T, class U = double> constexpr T lerp(T v0, T v1, U t) {
  return (1 - t) * v0 + t * v1;
}

// template <class T, class U = double> constexpr T cerp(T v0, T v1, U t) {
//   return lerp(v0, v1, -cos(M_PI * t) / 2 + 0.5);
// }

#endif /* end of include guard: __MATH_UTILITY__ */

/** @} */ // end of math
