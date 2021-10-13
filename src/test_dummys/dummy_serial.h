#pragma once

#include "../console_logger.h"

inline void delayMicroseconds(int delay) {}

class DummySerial {
public:

  std::string last_print;

  bool available() {
    return true;
  }

  uint8_t read() {
    return 45;
  }

  void printf(const char * format, ...) {
    va_list args;
    va_start (args, format);
    size_t len = vsnprintf(NULL, 0, format, args);
    va_end (args);
    char vec[len + 1];
    va_start (args, format);
    vsnprintf(&vec[0], len + 1, format, args);
    va_end (args);
    std::stringstream sstr;
    sstr << vec;
    last_print = sstr.str();
    logger_i("DummySerial >>", last_print);
  }
};

DummySerial Serial;
