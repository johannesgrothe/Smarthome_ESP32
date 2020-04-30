#ifndef __SystemTimer__
#define __SystemTimer__

#include "Arduino.h"
#include "console_logger.h"

static void printULongLong(const unsigned long long input) {
  unsigned long long buf = input;
  char bufarr[15]{};
  int pointer = 13;
  while (buf > 0) {
    char bufchr[3]{};
    sprintf(bufchr, "%d", ((int) (buf % 10)));
    bufarr[pointer] = bufchr[0];
    buf = buf / 10;
    pointer--;
  }
  while (pointer >= 0) {
    bufarr[pointer] = ' ';
    pointer--;
  }
  Serial.println(bufarr);
}

class SystemTimer {
private:
  unsigned long long time = 0;

public:
  SystemTimer() {};

  unsigned long long getTime() const {
    Serial.println((unsigned long) (time % 1000000));
    Serial.println((unsigned long) ((time + millis()) % 1000000));
    return time + millis();
  }

  void setTime(unsigned long long new_time) {
    time = new_time - millis();
//    Serial.println((unsigned long)(time % 1000000));
    logger.print("Setting System Time: ");
    printULongLong(new_time);
    logger.addln();
  }

};

SystemTimer system_timer;

#endif //__SystemTimer__