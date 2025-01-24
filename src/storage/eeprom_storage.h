#pragma once

#include "system_storage.h"
#include "eeprom_manager.h"


/**
 * System storage class handling EEPROM saving
 */
class EepromStorage final : public SystemStorage {
public:
    EepromStorage();

    /**
     * Prints the whole eeprom layout to the console
     */
    static std::string getEepromLayout();

    std::shared_ptr<SystemConfig> loadSystemConfig() override;

    bool saveSystemConfig(SystemConfig config) override;

    std::shared_ptr<EolConfig> loadEolConfig() override;

    bool saveEolConfig(EolConfig config) override;

    bool eraseAllConfigs() override;
};
