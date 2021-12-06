#pragma once

#include "pin_profile.h"
#include "hardware_controller.h"

#ifndef UNIT_TEST
#include <Arduino.h>
#define STATIC_BOOT_MODE 2
#endif

enum class BootMode {
  Serial_Only = 0,
  Network_Only = 1,
  Full_Operation = 2
};

static BootMode getBootMode() {
#ifdef STATIC_BOOT_MODE
  return BootMode(STATIC_BOOT_MODE);
#else
  HardwareController::setPinMode(REG0, INPUT);
  HardwareController::setPinMode(REG1, INPUT);
  HardwareController::setPinMode(REG2, INPUT);

  BootMode mode = BootMode::Serial_Only;

  bool reg0 = HardwareController::digitalReadPin(REG0);
  bool reg1 = HardwareController::digitalReadPin(REG1);
  bool reg2 = HardwareController::digitalReadPin(REG2);

  if (!reg0 && !reg1 && !reg2) {
    mode = BootMode::Serial_Only;
  } else if (!reg0 && reg1 && !reg2) {
    mode = BootMode::Network_Only;
  } else if (reg0 && reg1 && reg2) {
    mode = BootMode::Full_Operation;
  }
  return mode;
#endif
}
