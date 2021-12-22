#include "power_manager.h"


PowerManager::PowerManager(PowerMode mode):
  mode_(mode) {}

uint8_t PowerManager::getBatteryLevel() const {
  // TODO calculate battery level
  return 1;
}

PowerMode PowerManager::getPowerMode() const {
  return mode_;
}

uint8_t PowerManager::getVoltage() const {
  // TODO calculate current voltage
  return 50;
}




