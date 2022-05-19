#pragma once

#include "config.h"
#include "system_storage.h"

#include "eeprom_manager.h"


/**
 * System storage class handling EEPROM saving
 */
class EepromStorage: public SystemStorage {
public:

  EepromStorage();

  /**
   * Prints the whole eeprom layout to the console
   */
  static std::string getEepromLayout();

  /**
   * Returns the address of the last used byte in the eeprom.
   * The maximum eeprom is EEPROM_SIZE
   * @return the address of the last used byte in the eeprom
   */
  static uint16_t getEEPROMUsage();

  /**
   * Loads the system config from the EEPROM
   * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
   */
  std::shared_ptr<SystemConfig> loadSystemConfig() override;

  /**
   * Saves the systenm config to the EEPROM
   * @param config The config to write
   * @return Whether saving was successful
   */
  bool saveSystemConfig(SystemConfig config) override;

  /**
   * Loads the gadget config from the EEPROM
   * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
   */
  std::shared_ptr<GadgetConfig> loadGadgetConfig() override;

  /**
   * Saves a gadget config to the EEPROM
   * @param config The config to write
   * @return Whether saving was successful
   */
  bool saveGadgetConfig(GadgetConfig config) override;

  /**
   * Loads the event config from the EEPROM
   * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
   */
  std::shared_ptr<EventConfig> loadEventConfig() override;

  /**
   * Saves a event config to the EEPROM
   * @param config The config to write
   * @return Whether saving was successful
   */
  bool saveEventConfig(EventConfig config) override;

  bool eraseAllConfigs() override;

  void setDoubleRebootFlag(bool value) override;

  bool getDoubleRebootFlag() override;
};
