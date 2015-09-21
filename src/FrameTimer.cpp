/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file FrameTimer.cpp
*   \author Jan-Niklas Braak
*/

#include "FrameTimer.hpp"
#include <sstream>

// FrameTimer::FrameTimer(std::chrono::duration<double> targetTime)
//     : targetTime(targetTime) {}
FrameTimer::FrameTimer(float targetTime) : targetTime(targetTime) {}
// FrameTimer::FrameTimer() {}
FrameTimer::~FrameTimer() {}

void FrameTimer::startFrame() {
  startOfFrame = std::chrono::high_resolution_clock::now();
}
void FrameTimer::endFrame(bool wait) {
  if(wait){
    std::this_thread::sleep_for(targetTime);
  }

  std::chrono::high_resolution_clock::time_point endOfFrame;
  endOfFrame = std::chrono::high_resolution_clock::now();
  frameTime = endOfFrame - startOfFrame;
}


float FrameTimer::getFps() { return 1.0 / frameTime.count(); }
float FrameTimer::getFrameTime() { return frameTime.count(); }
std::string FrameTimer::getOutput() {
  std::stringstream outputString;
  outputString << getFps() << "(" << getFrameTime() * 1000 << " ms)";
  return outputString.str();
}
