#pragma once

#include "config.h"

class SystemStorage {
protected:
  bool initialized_;

public:

  /**
   * Constructor for this meta class
   */
  SystemStorage();

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
  virtual bool saveConfig(Config config) = 0;

  /**
   * Erases the Config and whatever data was stored from the storage permanently
   * @return Whether erasing was successful
   */
  virtual bool eraseConfig() = 0;
};