#pragma once

inline void delayMicroseconds(int delay) {}

class DummySerial {
public:
  bool available() {
    return true;
  }

  uint8_t read() {
    return 45;
  }

  void printf(const char * message, ...) {

  }

};

DummySerial Serial;
