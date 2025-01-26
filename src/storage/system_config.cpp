#include "system_config.h"

#include <utility>
#include "../crc_calculator.h"
#include "../console_logger.h"

SystemConfig::SystemConfig(std::string id,
                           std::shared_ptr<std::string> wifi_ssid,
                           std::shared_ptr<std::string> wifi_pw,
                           std::shared_ptr<IPContainer> mqtt_ip,
                           std::shared_ptr<uint16_t> mqtt_port,
                           std::shared_ptr<std::string> mqtt_username,
                           std::shared_ptr<std::string> mqtt_password) :
    id(std::move(id)),
    wifi_ssid(std::move(wifi_ssid)),
    wifi_pw(std::move(wifi_pw)),
    mqtt_ip(std::move(mqtt_ip)),
    mqtt_port(std::move(mqtt_port)),
    mqtt_username(std::move(mqtt_username)),
    mqtt_password(std::move(mqtt_password)) {}

unsigned short SystemConfig::crc16() const {
  CRC16Calculator crc;

  // Add ID
  for (const char c: id) {
    crc.add(static_cast<uint8_t>(c));
  }

  // Add Wifi SSID
  if (wifi_ssid) {
    for (const auto chr: *wifi_ssid) {
      crc.add(static_cast<uint8_t>(chr));
    }
  } else {
    crc.add(0);
  }

  // Add Wifi PW
  if (wifi_pw) {
    for (const auto chr: *wifi_pw) {
      crc.add(static_cast<uint8_t>(chr));
    }
  } else {
    crc.add(0);
  }

  // Add MQTT IP
  if (mqtt_ip) {
    crc.add(mqtt_ip->getData()[0]);
    crc.add(mqtt_ip->getData()[1]);
    crc.add(mqtt_ip->getData()[2]);
    crc.add(mqtt_ip->getData()[3]);
  } else {
    crc.add(0);
  }

  // Add MQTT port
  if (mqtt_port) {
    crc.add(static_cast<uint8_t>(*mqtt_port) & 0xFF);
    crc.add(static_cast<uint8_t>(*mqtt_port) >> 8);
  } else {
    crc.add(0);
  }

  // Add MQTT username
  if (mqtt_username) {
    for (const auto chr: *mqtt_username) {
      crc.add(static_cast<uint8_t>(chr));
    }
  } else {
    crc.add(0);
  }

  // Add MQTT password
  if (mqtt_password) {
    for (const auto chr: *mqtt_password) {
      crc.add(static_cast<uint8_t>(chr));
    }
  } else {
    crc.add(0);
  }

  return crc.value();
}
