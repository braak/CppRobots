/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file FrameTimer.cpp
*   \author Jan-Niklas Braak
*/

#include <chrono>
#include <thread>
#include <string>

class FrameTimer {
private:
  std::chrono::duration<double> frameTime;
  std::chrono::duration<double> targetTime;
  std::chrono::high_resolution_clock::time_point startOfFrame;

public:
  // FrameTimer(std::chrono::duration<double> targetTime);
  // FrameTimer(float targetTime);
  FrameTimer();
  virtual ~FrameTimer();

  void startFrame();
  void endFrame();

  float getFps();
  float getFrameTime();
  std::string getOutput();
};
