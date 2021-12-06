#pragma once

//#define STATIC_CONFIG_ACTIVE

#include "../api/config_decoder.h"
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
  std::shared_ptr<SystemConfig> loadSystemConfig() override;

  /**
   * In static config mode, configs cannot be saved. Always returns false.
   * @param config Config one would want to save
   * @return Always False.
   */
  bool saveSystemConfig(SystemConfig config) override;

  /**
   * Loads the gadget config from the static config string
   * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
   */
  std::shared_ptr<GadgetConfig> loadGadgetConfig() override;

  /**
   * In static config mode, configs cannot be saved. Always returns false.
   * @param config Config one would want to save
   * @return Always False.
   */
  bool saveGadgetConfig(GadgetConfig config) override;

  /**
   * Loads the event config from the static config string
   * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
   */
  std::shared_ptr<EventConfig> loadEventConfig() override;

  /**
   * In static config mode, configs cannot be saved. Always returns false.
   * @param config Config one would want to save
   * @return Always False.
   */
  bool saveEventConfig(EventConfig config) override;

  bool eraseAllConfigs() override;
};