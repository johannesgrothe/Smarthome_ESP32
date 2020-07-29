#include "wifi_gadget.h"

WiFiGadget::WiFiGadget() = default;

WiFiGadget::WiFiGadget(JsonObject json) {
  if (json.isNull() || !json.containsKey("wifi_ssid") || !json.containsKey("wifi_password")) {
    logger.println(LOG_TYPE::ERR, "No valid wifi configuration.");
    wifi_initialized_ = false;
    return;
  }
  logger.println("Connecting to WiFi:");
  logger.incIndent();
  network_client_ = WiFiClient();
  const char *ssid = json["wifi_ssid"].as<char *>();
  const char *passwd = json["wifi_password"].as<char *>();

  if (ssid == nullptr || passwd == nullptr) {
    logger.println(LOG_TYPE::ERR, "Missing Username or Password.");
    logger.decIndent();
    wifi_initialized_ = false;
    return;
  }

  strncpy(wifi_ssid_, ssid, WIFI_SSID_LEN_MAX);
  strncpy(wifi_password_, passwd, WIFI_PASSWORD_LEN_MAX);

  logger.print(LOG_TYPE::DATA, "");
  logger.add("Connecting to ");
  logger.add(ssid);

  byte connection_tries = 0;

  while (WiFiClass::status() != WL_CONNECTED && connection_tries < 6) {
    WiFi.begin(ssid, passwd);
    delay(1000);
    logger.add(".");
    connection_tries++;
  }
  logger.addln();
  if (WiFiClass::status() != WL_CONNECTED) {
    logger.println(LOG_TYPE::DATA, "could not establish WiFi Connection...");
  } else {
    randomSeed(micros());
    logger.println(LOG_TYPE::DATA, "WiFi connected");
    logger.print(LOG_TYPE::DATA, "IP: ");
    logger.addln(WiFi.localIP().toString().c_str());
  }
  wifi_initialized_ = true;
  logger.decIndent();
}

bool WiFiGadget::wifiIsInitialized() const {
  return wifi_initialized_;
}