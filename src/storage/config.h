#pragma once

#include <vector>
#include <tuple>
#include <datatypes.h>
#include "../optional.h"
#include "../network_library.h"


class Config {
private:
  std::string id_;
  NetworkMode network_mode_;
  std::vector<gadget_tuple> gadgets_;

  Optional<uint8_t> ir_recv_pin_;
  Optional<uint8_t> ir_send_pin_;

  Optional<uint8_t> radio_recv_pin_;
  Optional<uint8_t> radio_send_pin_;

  Optional<std::string> wifi_ssid_;
  Optional<std::string> wifi_pw_;

  Optional<IPAddress> mqtt_ip_;
  Optional<uint16_t> mqtt_port_;

  Optional<std::string> mqtt_username_;
  Optional<std::string> mqtt_password_;

public:

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
  Config(std::string id,
         NetworkMode network_mode,
         std::vector<gadget_tuple> gadgets,
         Optional<uint8_t> ir_recv_pin,
         Optional<uint8_t> ir_send_pin,
         Optional<uint8_t> radio_recv_pin,
         Optional<uint8_t> radio_send_pin,
         Optional<std::string> wifi_ssid,
         Optional<std::string> wifi_pw,
         Optional<IPAddress> mqtt_ip,
         Optional<uint16_t> mqtt_port,
         Optional<std::string> mqtt_username,
         Optional<std::string> mqtt_password);

  /**
   * Returns all gadgets
   * @return A vector containing all gadget information
   */
  std::vector<gadget_tuple> getGadgets() const;

  /**
   * Collects all used ports from all gadgets
   * @return The used ports
   */
  std::vector<uint8_t> getGadgetPorts() const;

  /**
   * Collects all the names from the gadgets
   * @return The gadget names
   */
  std::vector<std::string> getGadgetNames() const;

  /**
   * Reads the ir receive pin
   * @return The ir recv pin
   */
  Optional<uint8_t> getIRRecvPin() const;

  /**
   * Reads the ir send pin
   * @return The ir send pin
   */
  Optional<uint8_t> getIRSendPin() const;

  /**
   * Reads the radio receiver pin
   * @return The radio receiver pin
   */
  Optional<uint8_t> readRadioRecvPin() const;

  /**
   * Reads the radio send pin
   * @return The radio send pin
   */
  Optional<uint8_t> readRadioSendPin() const;

  /**
   * Reads the network mode
   * @return The network mode
   */
  NetworkMode getNetworkMode() const;

  /**
   * Reads the chip identifier
   * @return The chip identifier
   */
  std::string getID() const;

  /**
   * Reads the WIFI SSID
   * @return The wifi ssid
   */
  Optional<std::string> getWifiSSID() const;

  /**
   * Reads the WIFI Password
   * @return The wifi password
   */
  Optional<std::string> getWifiPW() const;

  /**
   * Reads the MQTT IP-Address
   * @return The mqtt ip
   */
  Optional<IPAddress> getMqttIP() const;

  /**
   * Reads the MQTT port
   * @return The port
   */
  Optional<uint16_t> getMqttPort() const;

  /**
   * Reads the MQTT username
   * @return The username
   */
  Optional<std::string> getMqttUsername() const;

  /**
   * Reads the MQTT password
   * @return The password
   */
  Optional<std::string> getMqttPassword();
};
