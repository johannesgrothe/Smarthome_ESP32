#include "static_storage.h"
#include <sstream>
#include "../console_logger.h"

StaticStorage::StaticStorage() {
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

  DynamicJsonDocument static_json_config(5000);

  auto err = deserializeJson(static_json_config, ss.str());
  if (err != DeserializationError::Ok) {

    logger_e("StaticStorage", "Could not load static config: Deserialization Error");
    return nullptr;
  }

  auto config = ConfigDecoder::decodeSystemConfig(static_json_config);

  if (config == nullptr) {
    logger_e("StaticStorage", "Could not load static config: Failed to create config from json");
  }

  return config;
}

bool StaticStorage::saveSystemConfig(SystemConfig config) {
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

std::shared_ptr<GadgetConfig> StaticStorage::loadGadgetConfig() {
  return nullptr;
}

bool StaticStorage::saveGadgetConfig(GadgetConfig config) {
  return false;
}

std::shared_ptr<EventConfig> StaticStorage::loadEventConfig() {
  return nullptr;
}

bool StaticStorage::saveEventConfig(EventConfig config) {
  return false;
}
