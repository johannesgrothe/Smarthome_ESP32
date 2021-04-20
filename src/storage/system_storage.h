#pragma once

#include "eeprom_storage.h"
#include "static_storage.h"
#include "system_storage_handler.h"

#include "config.h"
#include "../datatypes.h"
#include "../status_codes.h"

/**
 * System storage class handling EEPROM saving
 */
class System_Storage {
public:

  std::shared_ptr<SystemStorageHandler> handler;

  System_Storage():
  handler(nullptr) {
    #ifdef STATIC_CONFIG_ACTIVE
    logger.println("Using static, pre-compiled config");
    handler = std::make_shared<StaticStorage>();
    #else
    logger.println("Using dynamic, EEPROM config");
    handler = std::make_shared<EEPROM_Storage>();
    #endif
    if (!handler->isInitialized()) {
      handler = nullptr;
    }
  }

  /**
   * Loads the system config (either from EEPROM or static config)
   * @return The loaded Config
   */
  std::shared_ptr <Config> loadConfig() {
    if (handler == nullptr) {
      return nullptr;
    }
    return handler->loadConfig();
  }

  /**
   * Saves the passed config to the EEPROM. Returns false if the Chip runs in static config mode.
   * @param config The config to save
   * @return Whether saving was successful
   */
   bool saveConfig(Config config) {
    if (handler == nullptr) {
      return false;
    }
    return handler->writeConfig(config);
  }

  /**
   * Returns whether the system sorage was successfully initialized
   * @return Initialization status
   */
  bool isInitialized() const {
    return handler != nullptr;
  }
};
