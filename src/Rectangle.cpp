/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Rectangle.cpp
*   \author Jan-Niklas Braak
*/

#include "Rectangle.hpp"

Rectangle::Rectangle(Vector_d size, Vector_d position, double rotation)
    : size(size), origin({size.x / 2, size.y / 2}), position(position),
      rotation(rotation) {}

Rectangle::~Rectangle() {}

Rectangle &Rectangle::move(const Vector_d &delta) {
  position += delta;
  return *this;
}
/**
  Set the position of the Rectangle.
  \param position_ the new position of the Rectangle.
  \return *this for method chaining.
*/
Rectangle &Rectangle::setPosition(const Vector_d &position_) {
  position = position_;
  return *this;
}

/**
Get the postion of the Rectangle.
\return the position of the Rectangle.
*/
const Vector_d &Rectangle::getPosition() const { return position; }

/**
  Rotate the Rectangle by the given amount.
  \param delta amount to rotate the Vector
  \return *this for method chaining.
*/
Rectangle &Rectangle::rotate(double delta) {
  rotation = rotation + delta;
  return *this;
}

/**
  Set the rotation of the Rectangle.
  \param rotation_ the new rotation of the Rectangle.
  \return *this for method chaining.
*/
Rectangle &Rectangle::setRotation(double rotation_) {
  rotation = rotation_;
  return *this;
}

/**
Get the rotation of the Rectangle.
\return the rotation of the Rectangle.
*/
double Rectangle::getRotation() const { return rotation; }

/**
  Resize the Rectangle.
  \param delta the factor to resize the Rectangle.
  \return *this for method chaining.
*/
Rectangle &Rectangle::resize(double delta) {
  size = {size.x * delta, size.y * delta};
  return *this;
}
/**
  Resize the Rectangle.
  \param delta the factors to resize the Rectangle in both directions.
  \return *this for method chaining.
*/
Rectangle &Rectangle::resize(const Vector_d &delta) {
  size = {size.x * delta.x, size.y * delta.y};
  return *this;
}
/**
Set the size of the Rectangle.
\param size_ the new size of the Rectangle.
    \return *this for method chaining.
*/
Rectangle &Rectangle::setSize(const Vector_d &size_) {
  size = size_;
  return *this;
}

/**
Get the size of the Rectangle.
\return the size of the Rectangle.
*/
const Vector_d &Rectangle::getSize() const { return size; }

/**
  Returns the vertices of the rectangel, starting bottom-left (before
  rotation) and going counter-clockwise.
  \returns a vector of Points.
*/
const std::vector<Vector_d> Rectangle::vertices() const {
  std::vector<Vector_d> vertices(4);
  // top-left
  vertices[0] = toGlobal(Vector_d(0, 0) - origin);
  // top-right
  vertices[1] = toGlobal(Vector_d(size.x, 0) - origin);
  // bottom-right
  vertices[2] = toGlobal(Vector_d(size.x, size.y) - origin);
  // bottom-left
  vertices[3] = toGlobal(Vector_d(0, size.y) - origin);
  return vertices;
}

Vector_d Rectangle::toGlobal(Vector_d vec) const {
  return vec.rotate(rotation) + position;
}

std::ostream &operator<<(std::ostream &os, const Rectangle &obj) {
  os << "Rectangle(" << obj.getSize() << ", " << obj.getPosition() << ", "
     << obj.getRotation() << ")";
  return os;
}
