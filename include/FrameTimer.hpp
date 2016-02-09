/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file FrameTimer.hpp
*   \author Jan-Niklas Braak
*/
#ifndef __FRAME_TIMER__
#define __FRAME_TIMER__

#include <chrono>
#include <thread>
#include <string>

/**
\brief This class measures and controlls the frametiming of the simmulation.
*/
class FrameTimer {
private:
  std::chrono::duration<double> timeStep;
  std::chrono::duration<double> frameTime;
  std::chrono::high_resolution_clock::time_point startOfFrame;

public:
  /**
  Default destructor.
  */
  virtual ~FrameTimer();

  /**
  Set the targeted duration of frames.
  \param timeStep The targeted duration of frames.
  */
  void setTimeStep(std::chrono::duration<double> timeStep);
  /**
  This method is called to signal the transition to a new frame.
    \param wait when true sleep until next frame
  */
  void sync(bool wait = true);

  /**
  Returns the duration of the previous frame.
  */
  std::chrono::duration<double> getFrameTime();
  /**
  Returns the framerate and the frametime in a human readable format.
  \return s string in the form "<framerate> (<frametime> ms)"
  */
  std::string getOutput() const;
};

#endif /* end of include guard: __FRAME_TIMER__ */
