#pragma once

#ifdef STATIC_CONFIG_ACTIVE
#include "static_storage.h"
#else
#include "eeprom_storage.h"
#endif

#include "config.h"

/**
 * System storage class handling EEPROM saving
 */
class System_Storage {
public:

  /**
   * Loads the system config (either from EEPROM or static config)
   * @return The loaded Config
   */
  static Config loadConfig() {
    #ifdef STATIC_CONFIG_ACTIVE
    return Config(
          "not_implemented",
          NetworkMode::None,
          {},
          nullptr,
          nullptr,
          nullptr,
          nullptr,
          nullptr,
          nullptr,
          nullptr,
          nullptr,
          nullptr,
          nullptr
          );
    #else
    std::string k = "not_implemented";
    Optional<char *> i = nullptr;
    return Config(
          k,
          NetworkMode::None,
          std::vector<gadget_tuple>(),
          i,
          i,
          i,
          i,
          i,
          i,
          i,
          i,
          i,
          i

//          nullptr,
//          nullptr,
//          nullptr,
//          nullptr,
//          nullptr,
//          nullptr,
//          nullptr,
//          nullptr,
//          nullptr,
//          nullptr
          );
    #endif
  }

  /**
   * Saves the passed config to the EEPROM. Returns false if the Chip runs in static config mode.
   * @param config The config to save
   * @return Whether saving was successful
   */
  static bool saveConfig(Config config) {
    return false;
  }
};
