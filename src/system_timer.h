#ifndef __SystemTimer__
#define __SystemTimer__

#include "Arduino.h"
#include "console_logger.h"

class SystemTimer {
private:
  unsigned long long time = 0;

public:
  SystemTimer() {};

  unsigned long long getTime() const {
    return time + millis();
  }

  void setTime(const unsigned long long new_time, const unsigned long time_offset=0) {
    time = new_time - millis() + time_offset;
    logger.print("Setting System Time: ");
    logger.add(new_time);
    logger.add(" (+");
    logger.add(time_offset);
    logger.addln(")");
    logger.incIndent();
    logger.print(LOG_DATA, "System Launch Time: ");
    logger.addln(time);
    logger.print(LOG_DATA, "Momentary System Time: ");
    logger.addln(getTime());
  }

};

SystemTimer system_timer;

#endif //__SystemTimer__