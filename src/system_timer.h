#ifndef __SystemTimer__
#define __SystemTimer__

#include "Arduino.h"

class SystemTimer {
private:
  unsigned long time_index = 0;
  unsigned long time_offset = 0;

public:
  SystemTimer() {};

  unsigned long getTime() {
    return time_index - time_offset + millis();
  }

  void setTime(unsigned long index, unsigned long offset) {
    time_index = index;
    time_offset = offset;
  }

};

SystemTimer system_timer;

#endif //__SystemTimer__