#pragma once

#include <Arduino.h>

static void sysPinMode(__uint8_t pin, __uint8_t mode) {
  pinMode(pin, mode);
}

static void sysDigitalWrite(__uint8_t pin, __uint8_t val) {
  digitalWrite(pin, val);
}

static int sysDdigitalRead(__uint8_t pin) {
  return digitalRead(pin);
}