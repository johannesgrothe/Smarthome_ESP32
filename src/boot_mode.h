#pragma once

#include "pin_profile.h"

#ifndef UNIT_TEST
#include <Arduino.h>
#endif

//#define STATIC_BOOTMODE 2

enum class BootMode {
  Serial_Only = 0,
  Network_Only = 1,
  Full_Operation = 2
};

static BootMode getBootMode() {
  #ifndef UNIT_TEST
#ifdef STATIC_BOOTMODE
  return BootMode(STATIC_BOOTMODE);
#else
  pinMode(REG0, INPUT);
  pinMode(REG1, INPUT);
  pinMode(REG2, INPUT);

  BootMode mode = BootMode::Serial_Only;

  bool reg0 = digitalRead(REG0);
  bool reg1 = digitalRead(REG1);
  bool reg2 = digitalRead(REG2);

  if (!reg0 && !reg1 && !reg2) {
    mode = BootMode::Serial_Only;
  } else if (!reg0 && reg1 && !reg2) {
    mode = BootMode::Network_Only;
  } else if (reg0 && reg1 && reg2) {
    mode = BootMode::Full_Operation;
  }
  return mode;
#endif
  #else
  return BootMode::Full_Operation;
  #endif
}
