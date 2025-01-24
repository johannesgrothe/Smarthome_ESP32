#pragma once

#include "system_config.h"
#include "eol_config.h"

class SystemStorage {
protected:
    bool initialized_;

public:
    virtual ~SystemStorage() = default;

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
     * Loads the eol config from the EEPROM
     * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
     */
    virtual std::shared_ptr<EolConfig> loadEolConfig() = 0;

    /**
     * Saves a eol config to the EEPROM
     * @param config The config to write
     * @return Whether saving was successful
     */
    virtual bool saveEolConfig(EolConfig config) = 0;

    /**
     * Erases all configs and whatever data was stored from the storage permanently
     * @return Whether erasing was successful
     */
    virtual bool eraseAllConfigs() = 0;
};
