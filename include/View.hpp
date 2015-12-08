/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file View.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __CPPROBOTS_VIEW__
#define __CPPROBOTS_VIEW__

#include "Simulation.hpp"

#include <memory>

/**
  Abstract class for displaying the Simulation.
*/
class View {
public:
  /**
    Set wich Simulation to display.
  */
  virtual void setSimulation(std::shared_ptr<const Simulation> sim) = 0;

  /**
    Handles all input from the user.
  */
  virtual void input() = 0;
  /**
    Handles all output.
  */
  virtual void output() = 0;
  /**
    Finisches the User interaction.
  */
  virtual void finish() = 0;
  /**
    Tells wheter the View is running.
    \return true if the View is active, false otherwise.
  */
  virtual bool isRunning() const = 0;
  /**
    Add a line of text to the log.
    \param text The line of text to add to the log.
  */
  virtual void log(std::string text) = 0;
};

#endif /* end of include guard: __CPPROBOTS_VIEW__ */
