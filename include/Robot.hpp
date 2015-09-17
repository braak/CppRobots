/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file Robot.hpp
*   \author Jan-Niklas Braak
*/

#include <math.h>
#include <string>

class Robot {
private:
  double x;
  double y;
  double theta;

  const double timeBase;

public:
  Robot(const double timeBase);
  virtual ~Robot();

  void update(double v, double w);

  operator std::string();
};
