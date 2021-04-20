#pragma once

#include "../json_check.h"

#include "eeprom_storage.h"
#include "static_storage.h"

#include "config.h"
#include "../datatypes.h"
#include "../status_codes.h"

/**
 * System storage class handling EEPROM saving
 */
class System_Storage {
public:

  System_Storage() {

  }

  /**
   * Loads the system config (either from EEPROM or static config)
   * @return The loaded Config
   */
  static std::shared_ptr <Config> loadConfig() {
    #ifdef STATIC_CONFIG_ACTIVE
    return StaticStorage::loadConfig();
    #else
    return EEPROM_Storage::loadConfig();
    #endif
  }

  /**
   * Saves the passed config to the EEPROM. Returns false if the Chip runs in static config mode.
   * @param config The config to save
   * @return Whether saving was successful
   */
  static bool saveConfig(Config config) {
    #ifdef STATIC_CONFIG_ACTIVE
    logger.println("Config is not saved: Using static config.");
    return false;
    #else
    logger.println(LOG_TYPE::ERR, "Saving config isn't implemented");
    return false;
    #endif
  }
};
