#pragma once

#include <PubSubClient.h>
#include "request_gadget.h"
#include <cstring>
#include "smarthome_remote_helper.h"
#include "wifi_gadget.h"

class MQTT_Gadget;

// Gadget to communicate with MQTT Endpoint
class MQTT_Gadget : public WiFiGadget, public Request_Gadget {
protected:

  IPAddress *mqttServer_{};

  PubSubClient *mqttClient_{};

  uint16_t mqtt_port_{};

  char username_[MQTT_USERNAME_MAX_LEN]{};

  char password_[MQTT_PW_MAX_LEN]{};

  bool connect_mqtt();

  void callback(char *, const byte *, unsigned int);

  void executeRequestSending(Request *) override;

public:
  MQTT_Gadget();

  explicit MQTT_Gadget(JsonObject);

  void refresh() override;
};
