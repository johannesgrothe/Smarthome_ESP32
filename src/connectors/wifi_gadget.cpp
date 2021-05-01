#include "wifi_gadget.h"

static const char *TAG = "WiFiGadget";

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
    logger_i(TAG, ".");
    connection_tries++;
  }
  if (WiFiClass::status() != WL_CONNECTED) {
    logger_e(TAG, "Could not establish WiFi Connection...");
  } else {
    randomSeed(micros());
    logger_i(TAG, "WiFi connected");
    logger_i(TAG, "IP: %s", WiFi.localIP().toString().c_str());
  }
  wifi_initialized_ = true;
}

bool WiFiGadget::wifiIsInitialized() const {
  return wifi_initialized_;
}
