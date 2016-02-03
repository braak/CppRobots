/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Rectangle.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __CPPROBOT_RECTANGLE__
#define __CPPROBOT_RECTANGLE__

#include <vector>
#include <math.h>
#include <ostream>

#include "Vector.hpp"
#include "Angles.hpp"

/**
  A Rectangle.
*/
class Rectangle {
  // local coordinates
  Vector_d size;
  Vector_d origin;

  // transformation
  Vector_d position;
  double rotation;

public:
  /**
  Constructor.
  */
  Rectangle(Vector_d size, Vector_d position = {0, 0}, double rotation = 0);
  /**
  Destructor
  */
  virtual ~Rectangle();

  /**
    Move the Rectangle by the given Vector.
    \param delta amount to move the Vector
    \return *this for method chaining.
  */
  Rectangle &move(const Vector_d &delta);
  /**
    Set the position of the Rectangle.
    \param position_ the new position of the Rectangle.
    \return *this for method chaining.
  */
  Rectangle &setPosition(const Vector_d &position_);

  /**
  Get the postion of the Rectangle.
  \return the position of the Rectangle.
  */
  const Vector_d &getPosition() const;

  /**
    Rotate the Rectangle by the given amount.
    \param delta amount to rotate the Vector
    \return *this for method chaining.
  */
  Rectangle &rotate(double delta);

  /**
    Set the rotation of the Rectangle.
    \param rotation_ the new rotation of the Rectangle.
    \return *this for method chaining.
  */
  Rectangle &setRotation(double rotation_);

  /**
  Get the rotation of the Rectangle.
  \return the rotation of the Rectangle.
  */
  double getRotation() const;
  /**
    Resize the Rectangle.
    \param delta the factor to resize the Rectangle.
    \return *this for method chaining.
  */
  Rectangle &resize(double delta);
  /**
    Resize the Rectangle.
    \param delta the factors to resize the Rectangle in both directions.
    \return *this for method chaining.
  */
  Rectangle &resize(const Vector_d &delta);
  /**
  Set the size of the Rectangle.
  \param size_ the new size of the Rectangle.
      \return *this for method chaining.
  */
  Rectangle &setSize(const Vector_d &size_);

  /**
  Get the size of the Rectangle.
  \return the size of the Rectangle.
  */
  const Vector_d &getSize() const;

  /**
    Returns the vertices of the rectangel, starting bottom-left (before
    rotation) and going counter-clockwise.
    \returns a vector of Points.
  */
  const std::vector<Vector_d> vertices() const;

  // std::vector<Vector_d> normals() const {
  //   std::vector<Vector_d> normals(4);
  //   std::vector<Vector_d> verts = vertices();
  //   for (size_t i = 0; i < 4; i++) {
  //     // caluclate the edge between this vertex and the next.
  //     const Vector_d p1 = verts[i];
  //     const Vector_d p2 = verts[(i + 1) % 4];
  //     const Vector_d edge = p1 - p2;
  //     // caluclate the normalized perpendicular vector of the edge.
  //     normals[i] = {-edge.y / edge.magnitude(), edge.x / edge.magnitude()};
  //   }
  //   return normals;
  // }

  // Rectangle boundingBox() const;
private:
  Vector_d toGlobal(Vector_d vec) const;
};

/**
Output stream Operator.
\param os the output stream.
\param obj the Rectangle to output.
\return the output stream.
*/
std::ostream &operator<<(std::ostream &os, const Rectangle &obj);

#endif /* end of include guard: __CPPROBOT_RECTANGLE__ */
