#include "static_storage.h"

std::shared_ptr <Config> StaticStorage::loadConfig() {
  #ifdef STATIC_CONFIG_ACTIVE
  DynamicJsonDocument static_json_config(5000);

    auto err = deserializeJson(static_json_config, STATIC_CONFIG_STR);
    if (err != DeserializationError::Ok) {
      logger.printfln(LOG_TYPE::ERR, "Could not load static config: Deserialization Error");
      return nullptr;
    }

    auto config = createConfigFromJson(static_json_config);

    if (config == nullptr) {
      logger.printfln(LOG_TYPE::ERR, "Could not load static config: Config Loading Error");
    }

    return config;
  #else
  return nullptr;
  #endif
}

bool StaticStorage::writeConfig(Config config) {
  return false;
}
