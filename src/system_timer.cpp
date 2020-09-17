#include "system_timer.h"

SystemTimer::SystemTimer() = default;

unsigned long long SystemTimer::getTime() const {
  return time_ + millis();
}

void SystemTimer::setTime(const unsigned long long new_time, const unsigned long time_offset = 0) {
  time_ = new_time - millis() + time_offset;
  logger.print("Setting System Time: ");
  logger.print(new_time);
  logger.print(" (+");
  logger.print(time_offset);
  logger.println(")");
  logger.incIndent();
  logger.print(LOG_TYPE::DATA, "System Launch Time: ");
  logger.println(time_);
  logger.print(LOG_TYPE::DATA, "Momentary System Time: ");
  logger.println(getTime());
  logger.decIndent();
}

SystemTimer system_timer;
