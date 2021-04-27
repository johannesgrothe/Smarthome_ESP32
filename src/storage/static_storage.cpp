#include "static_storage.h"
#include <strstream>

StaticStorage::StaticStorage() {
  initialized_ = staticConfigStringAvailable();
}

std::shared_ptr <Config> StaticStorage::loadConfig() {

  std::stringstream ss;

  #ifdef STATIC_CONFIG_ACTIVE
  ss << STATIC_CONFIG_STR;
  #else
  logger.printfln(LOG_TYPE::ERR, "Could not load static config: No static config found");
  return nullptr;
  #endif

  DynamicJsonDocument static_json_config(5000);

  auto err = deserializeJson(static_json_config, ss.str());
  if (err != DeserializationError::Ok) {
    logger.log("StaticStorage", LOG_TYPE::ERR) << "Could not load static config: Deserialization Error\n";
    return nullptr;
  }

  auto config = createConfigFromJson(static_json_config);

  if (config == nullptr) {
    logger.log("StaticStorage", LOG_TYPE::ERR) << "Could not load static config: Failed to create config from json\n";
  }

  return config;
}

bool StaticStorage::saveConfig(Config config) {
  return false;
}

bool StaticStorage::staticConfigStringAvailable() {
  #ifdef STATIC_CONFIG_ACTIVE
  return true;
  #else
  return false;
  #endif
}
