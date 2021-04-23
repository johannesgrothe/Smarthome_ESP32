#include "system_storage.h"

bool SystemStorage::isInitialized() const {
  return initialized_;
}

SystemStorage::SystemStorage() :
    initialized_(false) {}
