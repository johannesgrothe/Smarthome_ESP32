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
  logger.log("SystemTimer") << "Setting System Time: " << new_time << " (+" << time_offset << ")\n";
  ++ logger;
  logger.log(LOG_TYPE::DATA) << "System Launch Time: " << time_ << "\n";
  logger.log(LOG_TYPE::DATA) << "Momentary System Time: " << getTime() << "\n";
  -- logger;
}

SystemTimer system_timer;
