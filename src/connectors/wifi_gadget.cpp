#include "wifi_gadget.h"

WiFiGadget::WiFiGadget() = default;

WiFiGadget::WiFiGadget(std::string ssid, std::string pw):
wifi_ssid_(std::move(ssid)),
wifi_password_(std::move(pw)) {
  logger_i(TAG, "Connecting to WiFi:");
  network_client_ = WiFiClient();

  if (wifi_ssid_ == "null" || wifi_password_ == "null") {
    logger_e(TAG, "Missing Username or Password.");
    wifi_initialized_ = false;
    return;
  }

  logger_i(TAG, "Connecting to %s", wifi_ssid_.c_str());

  byte connection_tries = 0;

  while (WiFiClass::status() != WL_CONNECTED && connection_tries < 6) {
    WiFi.begin(wifi_ssid_.c_str(), wifi_password_.c_str());
    delay(1000);
    logger << ".";
    connection_tries++;
  }
  logger << "\n";
  if (WiFiClass::status() != WL_CONNECTED) {
    logger.sender("WifiGadget").level(LOG_TYPE::DATA) << "Could not establish WiFi Connection...\n";
  } else {
    randomSeed(micros());
    logger.sender("WifiGadget").level(LOG_TYPE::DATA) << "WiFi connected\n";
    logger.sender("WifiGadget").level(LOG_TYPE::DATA) << "IP: " << WiFi.localIP().toString() << "\n";
  }
  wifi_initialized_ = true;
  -- logger;
}

bool WiFiGadget::wifiIsInitialized() const {
  return wifi_initialized_;
}
