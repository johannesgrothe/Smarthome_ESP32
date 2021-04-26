#include "system_timer.h"
#include <limits>

SystemTimer::SystemTimer() = default;

unsigned long long SystemTimer::getTime() {
  mtx.lock();
  auto buf_time = time_ + millis();

  // Detect if overflow has happened, compensate it if that's the case.
  if (last_time_ > buf_time) {
    time_ += std::numeric_limits<unsigned long long>::max();
    buf_time = time_ + millis();
  }

  last_time_ = buf_time;
  mtx.unlock();
  return buf_time;
}

void SystemTimer::setTime(const unsigned long long new_time, const unsigned long time_offset = 0) {
  mtx.lock();
  time_ = new_time - millis() + time_offset;
  mtx.unlock();
  logger.print("Setting System Time: ");
  logger.print(new_time);
  logger.print(" (+");
  logger.print(time_offset);
  logger.println(")");
  ++ logger;
  logger.print(LOG_TYPE::DATA, "System Launch Time: ");
  logger.println(time_);
  logger.print(LOG_TYPE::DATA, "Momentary System Time: ");
  logger.println(getTime());
  -- logger;
}

SystemTimer system_timer;
