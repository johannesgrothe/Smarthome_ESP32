#include "wifi_gadget.h"

#include <utility>

WiFiGadget::WiFiGadget() = default;

WiFiGadget::WiFiGadget(std::string ssid, std::string pw):
wifi_ssid_(std::move(ssid)),
wifi_password_(std::move(pw)) {
  logger.println("Connecting to WiFi:");
  logger.incIndent();
  network_client_ = WiFiClient();

  if (wifi_ssid_ == "null" || wifi_password_ == "null") {
    logger.println(LOG_TYPE::ERR, "Missing Username or Password.");
    logger.decIndent();
    wifi_initialized_ = false;
    return;
  }

  logger.print(LOG_TYPE::DATA, "");
  logger.printf("Connecting to %s", wifi_ssid_.c_str());

  byte connection_tries = 0;

  while (WiFiClass::status() != WL_CONNECTED && connection_tries < 6) {
    WiFi.begin(wifi_ssid_.c_str(), wifi_password_.c_str());
    delay(1000);
    logger.print(".");
    connection_tries++;
  }
  logger.printnl();
  if (WiFiClass::status() != WL_CONNECTED) {
    logger.println(LOG_TYPE::DATA, "could not establish WiFi Connection...");
  } else {
    randomSeed(micros());
    logger.println(LOG_TYPE::DATA, "WiFi connected");
    logger.print(LOG_TYPE::DATA, "IP: ");
    logger.println(WiFi.localIP().toString().c_str());
  }
  wifi_initialized_ = true;
  logger.decIndent();
}

bool WiFiGadget::wifiIsInitialized() const {
  return wifi_initialized_;
}
