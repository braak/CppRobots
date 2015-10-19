/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Rectangle.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __RECTANGLE__
#define __RECTANGLE__

#include <vector>
#include <math.h>
#include "Vector.hpp"
#include "Angles.hpp"

/**
  A Rectangle.
*/
class Rectangle {
  // local coordinates
  Vector_d size;
  // Vector_d center;

  // transformation
  Vector_d position;
  double rotation;

public:
  /**
  Constructor.
  */
  Rectangle(Vector_d size, Vector_d position = {0, 0}, double rotation = 0)
      : size(size), position(position), rotation(rotation) {}
  /**
  Destructor
  */
  virtual ~Rectangle() {}

  /**
    Move the Rectangle by the given Vector.
    \param delta amount to move the Vector
    \return *this for method chaining.
  */
  Rectangle &move(const Vector_d &delta) {
    position += delta;
    return *this;
  }
  /**
    Set the position of the Rectangle.
    \param position_ the new position of the Rectangle.
    \return *this for method chaining.
  */
  Rectangle &setPosition(const Vector_d &position_) {
    position = position_;
    return *this;
  }

  /**
  Get the postion of the Rectangle.
  \return the position of the Rectangle.
  */
  const Vector_d &getPosition() const { return position; }

  /**
    Rotate the Rectangle by the given amount.
    \param delta amount to rotate the Vector
    \return *this for method chaining.
  */
  Rectangle &rotate(double delta) {
    rotation = rotation + delta;
    return *this;
  }

  /**
    Set the rotation of the Rectangle.
    \param rotation_ the new rotation of the Rectangle.
    \return *this for method chaining.
  */
  Rectangle &setRotation(double rotation_) {
    rotation = rotation_;
    return *this;
  }

  /**
  Get the rotation of the Rectangle.
  \return the rotation of the Rectangle.
  */
  double getRotation() const { return rotation; }

  /**
    Resize the Rectangle.
    \param delta the factor to resize the Rectangle.
    \return *this for method chaining.
  */
  Rectangle &resize(double delta) {
    size = {size.x * delta, size.y * delta};
    return *this;
  }
  /**
    Resize the Rectangle.
    \param delta the factors to resize the Rectangle in both directions.
    \return *this for method chaining.
  */
  Rectangle &resize(const Vector_d &delta) {
    size = {size.x * delta.x, size.y * delta.y};
    return *this;
  }
  /**
  Set the size of the Rectangle.
  \param size_ the new size of the Rectangle.
      \return *this for method chaining.
  */
  Rectangle &setSize(const Vector_d &size_) {
    size = size_;
    return *this;
  }

  /**
  Get the size of the Rectangle.
  \return the size of the Rectangle.
  */
  const Vector_d &getSize() const { return size; }

  /**
    Returns the vertices of the rectangel, starting top-left(before rotation)
    and going clockwise.
    \returns a vector of Points.
  */
  const std::vector<Vector_d> vertices() const {
    std::vector<Vector_d> vertices(4);
    // top-left
    vertices[0] =
        Vector_d(-size.x / 2, -size.y / 2).rotate(rotation) + position;
    // top-right
    vertices[1] = Vector_d(size.x / 2, -size.y / 2).rotate(rotation) + position;
    // bottom-right
    vertices[2] = Vector_d(size.x / 2, size.y / 2).rotate(rotation) + position;
    // bottom-left
    vertices[3] = Vector_d(-size.x / 2, size.y / 2).rotate(rotation) + position;
    return vertices;
  }
  // const std::vector<Vector_d> normals() const {
  //   std::vector<Vector_d> normals(4);
  //   std::vector<Vector_d> verts = vertices();
  //   for (size_t i = 0; i < 4; i++) {
  //     const Vector_d p1 = verts[i];
  //     const Vector_d p2 = verts[(i + 1) % 4];
  //     const Vector_d edge = p1 - p2;
  //     normals[i] = {-edge.y, edge.x};
  //   }
  //   return normals;
  // }

  // Rectangle boundingBox() const;
};

#endif /* end of include guard: __RECTANGLE__ */
