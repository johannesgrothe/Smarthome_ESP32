#include "system_timer.h"

static const char *TAG = "SystemTimer";

SystemTimer::SystemTimer() = default;

unsigned long long SystemTimer::getTime() {
  #ifdef UNIT_TEST
  if (set_time_) {
    return set_time_;
  }
  #endif

  mtx.lock();
  auto buf_time = time_ + HardwareController::getMillis();

  // Detect if overflow has happened, compensate it if that's the case.
  if (last_time_ > buf_time) {
    time_ += std::numeric_limits<unsigned long long>::max();
    buf_time = time_ + HardwareController::getMillis();
  }

  last_time_ = buf_time;
  mtx.unlock();
  return buf_time;
}

void SystemTimer::setTime(const unsigned long long new_time, const unsigned long time_offset = 0) {
  mtx.lock();
  time_ = new_time - HardwareController::getMillis() + time_offset;
  mtx.unlock();
  logger_i(TAG, "Setting System Time: %d (+%d)", new_time, time_offset);
  logger_i(TAG, "System Launch Time: %d", time_);
  logger_i(TAG, "Momentary System Time: %d", getTime());
}

SystemTimer system_timer;
