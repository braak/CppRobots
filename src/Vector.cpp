/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Vector.cpp
*   \author Jan-Niklas Braak
*/

#include "Vector.hpp"

// Explicite template instantiation
template class Vector<int>;
template class Vector<unsigned int>;
template class Vector<float>;

// template <typename T> T magnitude(const Vector<T> &vec) {
//   return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
// }
// template <typename T> T angle(const Vector<T> &vec) {
//   return atan2(vec.y, vec.x);
// }
// template <typename T> T distance(const Vector<T> &vec1, const Vector<T>
// &vec2) {
//   return magnitude(vec2 - vec1);
// }
// Vector<T> normalize(const Vector<T> &vec) { return vec / magnitude(vec); }
