#pragma once

#include <storage/eol_config.h>
#include <storage/system_config.h>
#include <json_validator.h>

class ConfigDecoder {
public:
    /**
     * Decodes the json of a system config
     * @param config Config json document to decode
     * @return The decoded config
     */
    static std::shared_ptr<SystemConfig> decodeSystemConfig(DynamicJsonDocument config);

    /**
     * Decodes the json of a system config
     * @param config Config json document to decode
     * @return The decoded config
     */
    static std::shared_ptr<EolConfig> decodeEolConfig(DynamicJsonDocument config);
};
