/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file View.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __CPPROBOTS_VIEW__
#define __CPPROBOTS_VIEW__

class View {
public:
  virtual void setSimulation(std::shared_ptr<Simulation> sim) = 0;
  virtual void input() = 0;
  virtual void output() = 0;
  virtual void finish() = 0;
  virtual bool isRunning() const = 0;
  virtual void log(std::string text) = 0;
};

#endif /* end of include guard: __CPPROBOTS_VIEW__ */
