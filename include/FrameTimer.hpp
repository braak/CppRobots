/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file FrameTimer.cpp
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
  std::chrono::duration<double> frameTime;
  std::chrono::duration<double> targetTime;
  std::chrono::high_resolution_clock::time_point startOfFrame;

public:
  /**
  Constructs a FrameTimer object with the specified target time.
  \param targetTime the frame tiem targeted by the Frametimer.
  */
  FrameTimer(float targetTime);
  /**
  Default destructor.
  */
  virtual ~FrameTimer();

  /**
  This method is called to signal the start of a frame.
  */
  void startFrame();
  /**
  This method is called to signal the end of a frame.

  If wait is true it lets the current thread sleep until the targetTime is reached.

  \param wait Wheter to wait for the next frame or not.
  */
  void endFrame(bool wait = false);

  /**
  Returns the current framerate.
  \return the current framerate
  */
  float getFps();
  /**
  Returns the duration of the previous frame.
  */
  float getFrameTime();
  /**
  Returns the framerate and the frametime in a human readable format.
  \return s string in the form "<framerate> (<frametime> ms)"
  */
  std::string getOutput();
};

#endif /* end of include guard: __FRAME_TIMER__ */
