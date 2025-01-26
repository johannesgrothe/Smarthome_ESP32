#include "static_storage.h"
#include <sstream>
#include <ArduinoJson.h>
#include <api/coders/config_decoder.h>

#include "../console_logger.h"

StaticStorage::StaticStorage() {
    logger_i(TAG, "Initializing EEPROM Storage...");
    initialized_ = staticConfigStringAvailable();
}

std::shared_ptr<SystemConfig> StaticStorage::loadSystemConfig() {
    std::stringstream ss;

#ifdef STATIC_CONFIG_ACTIVE
    ss << STATIC_CONFIG_STR;
#else
    logger_e("StaticStorage", "Could not load static config: No static config found");
    return nullptr;
#endif

    DynamicJsonDocument static_json_config(400);

    auto err = deserializeJson(static_json_config, ss.str());
    if (err != DeserializationError::Ok) {
        logger_e("StaticStorage", "Could not load static config: Deserialization Error");
        return nullptr;
    }

    auto config = ConfigDecoder::decodeSystemConfig(static_json_config["system"]);

    if (config == nullptr) {
        logger_e("StaticStorage", "Could not load static system config: Failed to create config from json");
    }

    return config;
}

bool StaticStorage::saveSystemConfig(SystemConfig config) {
    return false;
}

std::shared_ptr<EolConfig> StaticStorage::loadEolConfig() {
    std::stringstream ss;

#ifdef STATIC_CONFIG_ACTIVE
    ss << STATIC_CONFIG_STR;
#else
    logger_e("StaticStorage", "Could not load static config: No static config found");
    return nullptr;
#endif

    DynamicJsonDocument static_json_config(100);

    auto err = deserializeJson(static_json_config, ss.str());
    if (err != DeserializationError::Ok) {
        logger_e("StaticStorage", "Could not load static config: Deserialization Error");
        return nullptr;
    }

    auto config = ConfigDecoder::decodeEolConfig(static_json_config["eol"]);

    if (config == nullptr) {
        logger_e("StaticStorage", "Could not load static eol config: Failed to create config from json");
    }

    return config;
}


bool StaticStorage::saveEolConfig(EolConfig config) {
    return false;
}

bool StaticStorage::staticConfigStringAvailable() {
#ifdef STATIC_CONFIG_ACTIVE
  return true;
#else
    return false;
#endif
}

bool StaticStorage::eraseAllConfigs() {
    return false;
}
