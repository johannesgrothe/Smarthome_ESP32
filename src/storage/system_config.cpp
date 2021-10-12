#include "system_config.h"
#include "../crc_calculator.h"

SystemConfig::SystemConfig(std::string id,
                           NetworkMode network_mode,
                           uint8_t ir_recv_pin,
                           uint8_t ir_send_pin,
                           uint8_t radio_recv_pin,
                           uint8_t radio_send_pin,
                           std::shared_ptr <std::string> wifi_ssid,
                           std::shared_ptr <std::string> wifi_pw,
                           std::shared_ptr <IPContainer> mqtt_ip,
                           std::shared_ptr <uint16_t> mqtt_port,
                           std::shared_ptr <std::string> mqtt_username,
                           std::shared_ptr <std::string> mqtt_password) :
    id(std::move(id)),
    network_mode(network_mode),
    ir_recv_pin(ir_recv_pin),
    ir_send_pin(ir_send_pin),
    radio_recv_pin(radio_recv_pin),
    radio_send_pin(radio_send_pin),
    wifi_ssid(std::move(wifi_ssid)),
    wifi_pw(std::move(wifi_pw)),
    mqtt_ip(std::move(mqtt_ip)),
    mqtt_port(std::move(mqtt_port)),
    mqtt_username(std::move(mqtt_username)),
    mqtt_password(std::move(mqtt_password)) {}

unsigned short SystemConfig::crc16() const {
  CRC16Calculator crc;

  // Add ID
  for (char c: id) {
    crc.add((uint8_t) c);
  }

  // Add network mode
  crc.add(uint8_t(network_mode));

  // Add IR/Radio pins
  crc.add(ir_recv_pin);
  crc.add(ir_send_pin);
  crc.add(radio_recv_pin);
  crc.add(radio_recv_pin);

  // Add Wifi SSID
  if (wifi_ssid) {
    for (auto chr: *wifi_ssid) {
      crc.add((uint8_t) chr);
    }
  } else {
    crc.add(0);
  }

  // Add Wifi PW
  if (wifi_pw) {
    for (auto chr: *wifi_pw) {
      crc.add((uint8_t) chr);
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
    crc.add((uint8_t) *mqtt_port & 0xFF);
    crc.add((uint8_t) *mqtt_port >> 8);
  } else {
    crc.add(0);
  }

  // Add MQTT username
  if (mqtt_username) {
    for (auto chr: *mqtt_username) {
      crc.add((uint8_t) chr);
    }
  } else {
    crc.add(0);
  }

  // Add MQTT password
  if (mqtt_password) {
    for (auto chr: *mqtt_password) {
      crc.add((uint8_t) chr);
    }
  } else {
    crc.add(0);
  }

  return crc.value();
}

bool SystemConfig::operator==(const SystemConfig &rhs) const {
  return crc16() == rhs.crc16();
}

bool SystemConfig::operator!=(const SystemConfig &rhs) const {
  return !(operator==(rhs));
}
