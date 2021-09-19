#pragma once

#include <chrono>

typedef std::chrono::system_clock Clock;

// inline keyword fixed the duplicate symbols problem
inline unsigned long millis() {
  auto now = Clock::now();
  auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
  auto fraction = now - seconds;
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
  return milliseconds.count();
}
