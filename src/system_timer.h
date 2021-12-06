#pragma once

#include <mutex>
#include <limits>
#include "console_logger.h"

#include "hardware_controller.h"


class SystemTimer {
private:

  // Reference value.
  // time_ + millis() is the 'real' current time
  unsigned long long time_ = 0;

  // Last accessed time.
  unsigned long long  last_time_ = 0;

  // Mutex to lock resources for multithreading
  std::mutex mtx;

  #ifdef UNIT_TEST
  unsigned long long set_time_{};
  #endif

public:
  SystemTimer();

  /**
   * Returns the current system time
   * @return The current system time
   */
  unsigned long long getTime();

  /**
   * Sets the system time reference
   * @param new_time The new system time
   * @param offset Offset to compensate for network delays
   */
  void setTime(unsigned long long new_time, unsigned long offset);

  #ifdef UNIT_TEST
  void lockTime(unsigned long long time) {
    set_time_ = time;
  }
  #endif
};

extern SystemTimer system_timer;
