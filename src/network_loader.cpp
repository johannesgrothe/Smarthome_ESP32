#include "network_loader.h"

#ifndef UNIT_TEST
#include "connectors/mqtt_gadget.h"
#endif

#include "connectors/serial_gadget.h"

std::shared_ptr<RequestGadget> NetworkLoader::loadNetwork(const Config& config) {
  auto mode = config.getNetworkMode();

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

std::shared_ptr<RequestGadget> NetworkLoader::loadMqtt(Config config) {
  #ifndef UNIT_TEST
  if (config.getWifiSSID() == nullptr) {
    logger_e("System", "Config has no valid wifi ssid");
    return nullptr;
  }

  if (config.getWifiPW() == nullptr) {
    logger_e("System", "Config has no valid wifi password");
    return nullptr;
  }

  if (config.getMqttIP() == nullptr) {
    logger_e("System", "Config has no valid mqtt ip");
    return nullptr;
  }

  if (config.getMqttPort() == nullptr) {
    logger_e("System", "Config has no valid mqtt port");
    return nullptr;
  }

  std::string ssid = *config.getWifiSSID();
  std::string wifi_pw = *config.getWifiPW();
  uint16_t port = *config.getMqttPort();
  IPContainer ip = *config.getMqttIP();

  if (!config.getMqttUsername() || config.getMqttPassword()) {
    logger_i("System", "Establishing MQTT connection without credentials");
    return std::make_shared<MQTTGadget>(config.getID(),
                                        ssid,
                                        wifi_pw,
                                        ip,
                                        port);
  } else {
    logger_i("System", "Establishing MQTT connection using credentials");
    std::string user = *config.getMqttUsername();
    std::string mqtt_pw = *config.getMqttPassword();

    return std::make_shared<MQTTGadget>(config.getID(),
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
