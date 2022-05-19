#pragma once

#include "system_config.h"
#include "gadget_config.h"
#include "event_config.h"

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
  virtual std::shared_ptr<SystemConfig> loadSystemConfig() = 0;

  /**
   * Saves a system config to the EEPROM
   * @param config The config to write
   * @return Whether saving was successful
   */
  virtual bool saveSystemConfig(SystemConfig config) = 0;

  /**
   * Loads the gadget config from the EEPROM
   * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
   */
  virtual std::shared_ptr<GadgetConfig> loadGadgetConfig() = 0;

  /**
   * Saves a gadget config to the EEPROM
   * @param config The config to write
   * @return Whether saving was successful
   */
  virtual bool saveGadgetConfig(GadgetConfig config) = 0;

  /**
 * Loads the event config from the EEPROM
 * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
 */
  virtual std::shared_ptr<EventConfig> loadEventConfig() = 0;

  /**
   * Saves a event config to the EEPROM
   * @param config The config to write
   * @return Whether saving was successful
   */
  virtual bool saveEventConfig(EventConfig config) = 0;

  /**
   * Erases all configs and whatever data was stored from the storage permanently
   * @return Whether erasing was successful
   */
  virtual bool eraseAllConfigs() = 0;

  /**
   * Sets the double reboot flag
   * @param value Value to set
   */
  virtual void setDoubleRebootFlag(bool value) = 0;

  /**
   * Reads the double reboot flag
   * @return Whether the flag is set or not
   */
  virtual bool getDoubleRebootFlag() = 0;
};
