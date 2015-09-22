/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Pose.hpp
*   \author Jan-Niklas Braak
*/
#ifndef __POSE__
#define __POSE__

#include <ostream>

/**
  \brief A Pose represents a location and orientation in a 2D space.

  The location is given by the vector \f$(x, y)\f$, the orientation is given by the angle theta (\f$\Theta\f$).
*/
class Pose {
public:
  double x; //!< The x-coordinate of the Pose.
  double y; //!< The y-coordinate of the Pose.
  double theta; //!< The orientation angle \f$\Theta\f$ of the Pose.

  /**
  Constructs a Pose.
  \param x The x-coordinate of the Pose.
  \param y The y-coordinate of the Pose.
  \param theta The orientation angle \f$\Theta\f$ of the Pose.

  */
  Pose(double x = 0, double y = 0, double theta = 0);
  virtual ~Pose();

  /**
  Elementwise addition of two Pose objects.
  */
  Pose& operator +=(const Pose&rhs);
  /**
  Elementwise subtraction of two Pose objects.
  */
  Pose& operator -=(const Pose&rhs);
  /**
  Elementwise multiplication of a Pose and a double.
  */
  Pose& operator *=(const double&rhs);
  /**
  Elementwise division of a Pose and a double.
  */
  Pose& operator /=(const double&rhs);
};

/**
  Output stream Operator.
*/
std::ostream& operator<<(std::ostream& os, const Pose& obj);

/**
  Binary elementwise addition of two Pose objects.
*/
Pose operator+(Pose lhs, const Pose& rhs);
/**
  Binary elementwise subtraction of two Pose objects.
*/
Pose operator-(Pose lhs, const Pose& rhs);
/**
  Binary elementwise multiplication of a Pose and a double.
*/
Pose operator*(Pose lhs, const double& rhs);
/**
  Binary elementwise multiplication of a double and a Pose.
*/
Pose operator*(const double& lhs, Pose rhs);
/**
  Binary elementwise division of a Pose and a double.
*/
Pose operator/(Pose lhs, const double& rhs);
/**
  Binary elementwise division of a double and a Pose.
*/
Pose operator/(const double& lhs, Pose rhs);

#endif /* end of include guard: __POSE__ */
