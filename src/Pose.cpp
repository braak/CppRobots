/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Pose.cpp
*   \author Jan-Niklas Braak
*/


#include "Pose.hpp"
#include <sstream>

Pose::Pose(double x, double y, double theta): x(x), y(y), theta(theta){
}

Pose::~Pose(){}


Pose& Pose::operator +=(const Pose&rhs){
  x += rhs.x;
  y += rhs.y;
  theta += rhs.theta;
  return *this;
}

Pose& Pose::operator -=(const Pose&rhs){
  x -= rhs.x;
  y -= rhs.y;
  theta -= rhs.theta;
  return *this;
}

Pose& Pose::operator *=(const double & rhs){
  x *= rhs;
  y *= rhs;
  theta *= rhs;
  return *this;
}
Pose& Pose::operator/=(const double&rhs){
  x /= rhs;
  y /= rhs;
  theta /= rhs;
  return *this;
}
Pose operator+(Pose lhs, const Pose& rhs){
  lhs += rhs;
  return lhs;
}
Pose operator-(Pose lhs, const Pose& rhs){
  lhs -= rhs;
  return lhs;
}
Pose operator*(Pose lhs, const double& rhs){
  lhs *= rhs;
  return lhs;
}
Pose operator*(const double& lhs, Pose rhs){
  rhs *= lhs;
  return rhs;
}
Pose operator/(Pose lhs, const double& rhs){
  lhs /= rhs;
  return lhs;
}
Pose operator/(const double& lhs, Pose rhs){
  rhs.x = lhs / rhs.x;
  rhs.y = lhs / rhs.y;
  rhs.theta = lhs / rhs.theta;
  return rhs;
}

std::ostream& operator<<(std::ostream& os, const Pose& obj){
  os << "(" << obj.x << ", " << obj.y << ", " << obj.theta << ")";
  return os;
}
