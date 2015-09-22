/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Pose.hpp
*   \author Jan-Niklas Braak
*/

#include <ostream>

class Pose {
public:
  double x;
  double y;
  double theta;

  Pose(double x = 0, double y = 0, double theta = 0);
  virtual ~Pose();

  Pose& operator +=(const Pose&rhs);
  Pose& operator -=(const Pose&rhs);
  Pose& operator *=(const double&rhs);
  Pose& operator /=(const double&rhs);
};

std::ostream& operator<<(std::ostream& os, const Pose& obj);
inline Pose operator+(Pose lhs, const Pose& rhs);
inline Pose operator-(Pose lhs, const Pose& rhs);
inline Pose operator*(Pose lhs, const double& rhs);
inline Pose operator*(const double& lhs, Pose rhs);
inline Pose operator/(Pose lhs, const double& rhs);
inline Pose operator/(const double& lhs, Pose rhs);
