#include "network_loader.h"

#ifndef UNIT_TEST
#include "connectors/mqtt_gadget.h"
#endif

#include "connectors/serial_gadget.h"

std::shared_ptr<RequestGadget> NetworkLoader::loadNetwork(const SystemConfig &config, NetworkMode mode) {
  if (mode == NetworkMode::None) {
    logger_e("System", "No network configured.");
    return nullptr;
  }

  // initialize Network
  if (mode == NetworkMode::MQTT) {
    return loadMqtt(config);
  } else if (mode == NetworkMode::Serial) {
    return loadSerial();
  }

  logger_e("System", "Unknown Network Settings");
  return nullptr;
}

std::shared_ptr<RequestGadget> NetworkLoader::loadMqtt(const SystemConfig& config) {
  #ifndef UNIT_TEST
  if (config.wifi_ssid == nullptr) {
    logger_e("System", "Config has no valid wifi ssid");
    return nullptr;
  }

  if (config.wifi_pw == nullptr) {
    logger_e("System", "Config has no valid wifi password");
    return nullptr;
  }

  if (config.mqtt_ip == nullptr) {
    logger_e("System", "Config has no valid mqtt ip");
    return nullptr;
  }

  if (config.mqtt_port == nullptr) {
    logger_e("System", "Config has no valid mqtt port");
    return nullptr;
  }

  std::string ssid = *config.wifi_ssid;
  std::string wifi_pw = *config.wifi_pw;
  uint16_t port = *config.mqtt_port;
  IPContainer ip = *config.mqtt_ip;

  if (!config.mqtt_username || config.mqtt_password) {
    logger_i("System", "Establishing MQTT connection without credentials");
    return std::make_shared<MQTTGadget>(config.id,
                                        ssid,
                                        wifi_pw,
                                        ip,
                                        port);
  } else {
    logger_i("System", "Establishing MQTT connection using credentials");
    std::string user = *config.mqtt_username;
    std::string mqtt_pw = *config.mqtt_password;

    return std::make_shared<MQTTGadget>(config.id,
                                        ssid,
                                        wifi_pw,
                                        ip,
                                        port,
                                        user,
                                        mqtt_pw);
  }
  #endif
  return nullptr;
}

std::shared_ptr<RequestGadget> NetworkLoader::loadSerial() {
  return std::make_shared<SerialGadget>();
}
