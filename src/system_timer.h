#ifndef __SystemTimer__
#define __SystemTimer__

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

static SystemTimer system_timer;

#endif //__SystemTimer__