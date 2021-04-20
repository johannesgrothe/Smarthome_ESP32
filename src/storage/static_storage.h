#pragma once

//#define STATIC_CONFIG_ACTIVE

#include "config_from_json.h"

#ifdef STATIC_CONFIG_ACTIVE
#include "static_config.h"
#endif

class StaticStorage {
public:

  /**
   * Loads the system config from the EEPROM
   * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
   */
  static std::shared_ptr<Config> loadConfig() {
    #ifdef STATIC_CONFIG_ACTIVE
    DynamicJsonDocument static_json_config(5000);

    auto err = deserializeJson(static_json_config, STATIC_CONFIG_STR);
    if (err != DeserializationError::Ok) {
      logger.printfln(LOG_TYPE::ERR, "Could not load static config: Deserialization Error");
      return nullptr;
    }

    auto config = createConfigFromJson(static_json_config);

    if (config == nullptr) {
      logger.printfln(LOG_TYPE::ERR, "Could not load static config: Config Loading Error");
    }

    return config;
    #else
    return nullptr;
    #endif
  }
};