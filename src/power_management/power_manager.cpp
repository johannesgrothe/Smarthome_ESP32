#include "power_manager.h"


PowerManager::PowerManager(PowerMode mode):
    battery_level_(0),
    battery_drain_(0),
    mode_(mode) {}

uint8_t PowerManager::getBatteryLevel() const {
  //TODO calculate battery level
  return battery_level_;
}

uint8_t PowerManager::getBatteryDrain() const {
  //TODO calculating battery drain
  return battery_drain_;
}


