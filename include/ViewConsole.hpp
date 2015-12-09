/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file ViewConsole.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __CPPROBOTS_VIEW_CONSOLE__
#define __CPPROBOTS_VIEW_CONSOLE__

#include "View.hpp"

/**
  A View that displays the state and progress of the simulation on the console.
*/
class ViewConsole : public View {
  std::shared_ptr<const Simulation> simulation;

public:
  void setSimulation(std::shared_ptr<const Simulation> sim) override;
  void input() override;
  void output() override;
  void finish() override;
  bool isRunning() const override;
  void log(std::string text) override;
};

#endif /* end of include guard: __CPPROBOTS_VIEW_CONSOLE__ */
