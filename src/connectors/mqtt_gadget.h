#pragma once

#include <PubSubClient.h>
#include "request_gadget.h"
#include <cstring>
#include "smarthome_remote_helper.h"
#include "wifi_gadget.h"

// Gadget to communicate with MQTT Endpoint
class MQTT_Gadget : public WiFiGadget, public Request_Gadget {
protected:

  IPAddress mqttServer_{};

  PubSubClient *mqttClient_{};

  uint16_t mqtt_port_{};

  std::string username_;

  std::string password_;

  bool has_credentials_;

  const std::string& client_name_;

  bool connect_mqtt();

  void callback(char *, const byte *, unsigned int);

  void executeRequestSending(Request *) override;

public:

  /**
   * Contrutctor for the mqtt gadget
   * @param client_name
   * @param wifi_ssid
   * @param wifi_pw
   * @param mqtt_ip
   * @param mqtt_port
   * @param mqtt_username
   * @param mqtt_pw
   */
  MQTT_Gadget(const std::string& client_name,
              std::string wifi_ssid,
              std::string wifi_pw,
              const IPAddress& mqtt_ip,
              uint16_t mqtt_port,
              const std::string& mqtt_username,
              const std::string& mqtt_pw);

  void refresh() override;
};
