#pragma once

#include "config.h"

class SystemStorageHandler {
protected:
  bool initialized_;

public:

  /**
   * Constructor for this meta class
   */
  SystemStorageHandler();

  /**
   * Whether the storage implementation was successfully initialized and is ready to use
   * @return Initialization status
   */
  bool isInitialized() const;

  /**
   * Loads the system config from the EEPROM
   * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
   */
  virtual std::shared_ptr<Config> loadConfig() = 0;

  /**
   * Saves the config to the EEPROM
   * @param config The config to write
   * @return Whether saving was successful
   */
  virtual bool writeConfig(Config config) = 0;
};