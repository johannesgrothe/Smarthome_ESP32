#pragma once

#include "Arduino.h"
#include "console_logger.h"

class SystemTimer {
private:
  unsigned long long time_ = 0;

public:
  SystemTimer();

  unsigned long long getTime() const;

  void setTime(unsigned long long, unsigned long);

};

extern SystemTimer system_timer;
