#pragma once

#include <PubSubClient.h>
#include "request_gadget.h"
#include <cstring>
#include "wifi_gadget.h"
#include "api/protocol_paths.h"
#include <IPAddress.h>
#include <ip_container.h>

// Gadget to communicate with MQTT Endpoint
class MQTTGadget : public WiFiGadget, public RequestGadget {
protected:

  IPContainer mqttServer_;

  PubSubClient *mqttClient_{};

  uint16_t mqtt_port_{};

  std::string username_;

  std::string password_;

  bool has_credentials_;

  const std::string& client_name_;

  bool connect_mqtt();

  void callback(char *, const byte *, unsigned int);

  void executeRequestSending(std::shared_ptr<Request> request) override;

  bool subscribe_to_topic(const std::string& topic);

  bool initMqttClient(const IPContainer& mqtt_ip, uint16_t mqtt_port);

public:

  /**
   * Constructor for the mqtt gadget
   * @param client_name
   * @param wifi_ssid
   * @param wifi_pw
   * @param mqtt_ip
   * @param mqtt_port
   * @param mqtt_username
   * @param mqtt_pw
   */
  MQTTGadget(const std::string& client_name,
             std::string wifi_ssid,
             std::string wifi_pw,
             const IPContainer& mqtt_ip,
             uint16_t mqtt_port,
             const std::string& mqtt_username,
             const std::string& mqtt_pw);

  /**
   * Constructor for the mqtt gadget
   * @param client_name
   * @param wifi_ssid
   * @param wifi_pw
   * @param mqtt_ip
   * @param mqtt_port
   */
  MQTTGadget(const std::string& client_name,
             std::string wifi_ssid,
             std::string wifi_pw,
             const IPContainer& mqtt_ip,
             uint16_t mqtt_port);

  void refresh_network() override;
};
