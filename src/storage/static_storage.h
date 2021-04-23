#pragma once

//#define STATIC_CONFIG_ACTIVE

#include "config_from_json.h"
#include "system_storage.h"

#ifdef STATIC_CONFIG_ACTIVE
#include "static_config.h"
#endif

class StaticStorage: public SystemStorage {
public:

  StaticStorage();

  /**
   * Checks whether a static config string is available for loading
   * @return Whether a static config string is available for loading
   */
  static bool staticConfigStringAvailable();

  /**
   * Loads the system config from the static config string
   * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
   */
  std::shared_ptr<Config> loadConfig() override;

  /**
   * In static config mode, configs cannot be saved. Always returns false.
   * @param config Config one would want to save
   * @return Always False.
   */
  bool saveConfig(Config config) override;
};