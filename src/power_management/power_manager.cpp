#include "power_manager.h"

PowerManager::PowerManager(PowerMode mode):
  mode_(mode){
}

PowerMode PowerManager::getPowerMode() const {
  return mode_;
}

bool PowerManager::refresh() {}





