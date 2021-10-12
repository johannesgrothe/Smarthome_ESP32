#pragma once

#include <memory>
#include "../datatypes.h"
#include "../network_mode.h"
#include "../ip_container.h"

class SystemConfig {
private:

public:

  const std::string id;
  const NetworkMode network_mode;

  const uint8_t ir_recv_pin;
  const uint8_t ir_send_pin;

  const uint8_t radio_recv_pin;
  const uint8_t radio_send_pin;

  const std::shared_ptr <std::string> wifi_ssid;
  const std::shared_ptr <std::string> wifi_pw;

  const std::shared_ptr <IPContainer> mqtt_ip;
  const std::shared_ptr <uint16_t> mqtt_port;

  const std::shared_ptr <std::string> mqtt_username;
  const std::shared_ptr <std::string> mqtt_password;

  /**
    * Constructor for the Config
    * @param id The ID of the chip
    * @param network_mode The network mode to boot in
    * @param gadgets List of gadgets configured on the system
    * @param ir_recv_pin Pin used by IR receiver
    * @param ir_send_pin Pin used by IR blaster
    * @param radio_recv_pin Pin used by Radio receiver
    * @param radio_send_pin Pin used by Radio sender
    * @param wifi_ssid SSID of the wifi to connect to
    * @param wifi_pw Password of the wifi to connect to
    * @param mqtt_ip IP of the MQTT broker to connect to
    * @param mqtt_port Port of the MQTT broker to connect to
    * @param mqtt_username Username used on the MQTT broker
    * @param mqtt_password Password used on the MQTT broker
    */
  SystemConfig(std::string id,
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
               std::shared_ptr <std::string> mqtt_password);

  unsigned short crc16() const;

  bool operator==(const SystemConfig& rhs) const;

  bool operator!=(const SystemConfig& rhs) const;
};
