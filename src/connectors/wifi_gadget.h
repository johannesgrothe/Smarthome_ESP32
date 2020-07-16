#pragma once

#include <ArduinoJson.h>
#include "console_logger.h"
#include "Client.h"
#include "WiFiClient.h"
#include <WiFi.h>

class WiFiGadget {
private:
  bool wifi_initialized_{};
protected:
  WiFiClient network_client_{};

  char wifi_ssid_[MQTT_USERNAME_MAX_LEN]{};
  char wifi_password_[MQTT_PW_MAX_LEN]{};

public:
  WiFiGadget();

  explicit WiFiGadget(JsonObject json);

  bool wifiIsInitialized() const;

};
