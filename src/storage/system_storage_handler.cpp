#include "system_storage_handler.h"

bool SystemStorageHandler::isInitialized() const {
  return initialized_;
}

SystemStorageHandler::SystemStorageHandler() :
    initialized_(false) {}
