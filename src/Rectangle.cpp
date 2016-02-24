/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
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

Rectangle &Rectangle::setPosition(const Vector_d &position_) {
  position = position_;
  return *this;
}

const Vector_d &Rectangle::getPosition() const { return position; }

Rectangle &Rectangle::rotate(double delta) {
  rotation = rotation + delta;
  return *this;
}

Rectangle &Rectangle::setRotation(double rotation_) {
  rotation = rotation_;
  return *this;
}

double Rectangle::getRotation() const { return rotation; }

Rectangle &Rectangle::resize(double delta) {
  size = {size.x * delta, size.y * delta};
  return *this;
}

Rectangle &Rectangle::resize(const Vector_d &delta) {
  size = {size.x * delta.x, size.y * delta.y};
  return *this;
}

Rectangle &Rectangle::setSize(const Vector_d &size_) {
  size = size_;
  return *this;
}

const Vector_d &Rectangle::getSize() const { return size; }

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
