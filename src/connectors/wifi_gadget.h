#pragma once

#include <WiFi.h>
#include <utility>
#include <ArduinoJson.h>
#include "console_logger.h"
#include "Client.h"
#include "WiFiClient.h"

class WiFiGadget {
private:
  bool wifi_initialized_{};
protected:
  WiFiClient network_client_{};

  std::string wifi_ssid_;
  std::string wifi_password_;

public:
  WiFiGadget();

  WiFiGadget(std::string ssid, std::string pw);

  bool wifiIsInitialized() const;

};
