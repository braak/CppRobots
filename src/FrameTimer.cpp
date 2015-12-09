/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file FrameTimer.cpp
*   \author Jan-Niklas Braak
*/

#include "FrameTimer.hpp"
#include <sstream>

// FrameTimer::FrameTimer(std::chrono::duration<double> timeStep)
//     : timeStep(timeStep),
//       startOfFrame(std::chrono::high_resolution_clock::now()) {}
FrameTimer::~FrameTimer() {}
void FrameTimer::setTimeStep(std::chrono::duration<double> timeStep_) {
  timeStep = timeStep_;
}

void FrameTimer::sync(bool wait) {
  if (wait) {
    std::this_thread::sleep_until(startOfFrame + timeStep);
  }

  auto endOfFrame = std::chrono::high_resolution_clock::now();
  frameTime = endOfFrame - startOfFrame;
  startOfFrame = endOfFrame;
}

std::chrono::duration<double> FrameTimer::getFrameTime() { return frameTime; }

std::string FrameTimer::getOutput() const {
  std::stringstream outputString;
  outputString << 1.0 / frameTime.count() << "(" << frameTime.count() * 1000
               << " ms)";
  return outputString.str();
}
