/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file FrameTimer.cpp
*   \author Jan-Niklas Braak
*/

#include "FrameTimer.hpp"
#include <sstream>

FrameTimer::FrameTimer(float targetTime)
    : targetTime(targetTime),
      startOfFrame(std::chrono::high_resolution_clock::now()) {}
FrameTimer::~FrameTimer() {}

void FrameTimer::startFrame(bool wait) {
  if (wait) {
    // auto wake_up_time = startOfFrame + targetTime;
    std::this_thread::sleep_until(startOfFrame + targetTime);
  }
  startOfFrame = std::chrono::high_resolution_clock::now();
}

void FrameTimer::endFrame() {
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
