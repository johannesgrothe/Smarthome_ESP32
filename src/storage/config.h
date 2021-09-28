#pragma once

#include <vector>
#include <tuple>
#include "../datatypes.h"
#include "../network_library.h"


class Config {
private:
  std::string id_;
  NetworkMode network_mode_;
  std::vector<gadget_tuple> gadgets_;

  uint8_t ir_recv_pin_;
  uint8_t ir_send_pin_;

  uint8_t radio_recv_pin_;
  uint8_t radio_send_pin_;

  std::shared_ptr<std::string> wifi_ssid_;
  std::shared_ptr<std::string> wifi_pw_;

  std::shared_ptr<IPAddress> mqtt_ip_;
  std::shared_ptr<uint16_t> mqtt_port_;

  std::shared_ptr<std::string> mqtt_username_;
  std::shared_ptr<std::string> mqtt_password_;

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
         uint8_t ir_recv_pin,
         uint8_t ir_send_pin,
         uint8_t radio_recv_pin,
         uint8_t radio_send_pin,
         std::shared_ptr<std::string> wifi_ssid,
         std::shared_ptr<std::string> wifi_pw,
         std::shared_ptr<IPAddress> mqtt_ip,
         std::shared_ptr<uint16_t> mqtt_port,
         std::shared_ptr<std::string> mqtt_username,
         std::shared_ptr<std::string> mqtt_password);

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
  uint8_t getIRRecvPin() const;

  /**
   * Reads the ir send pin
   * @return The ir send pin
   */
  uint8_t getIRSendPin() const;

  /**
   * Reads the radio receiver pin
   * @return The radio receiver pin
   */
  uint8_t getRadioRecvPin() const;

  /**
   * Reads the radio send pin
   * @return The radio send pin
   */
  uint8_t getRadioSendPin() const;

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
  std::shared_ptr<std::string> getWifiSSID() const;

  /**
   * Reads the WIFI Password
   * @return The wifi password
   */
  std::shared_ptr<std::string> getWifiPW() const;

  /**
   * Reads the MQTT IP-Address
   * @return The mqtt ip
   */
  std::shared_ptr<IPAddress> getMqttIP() const;

  /**
   * Reads the MQTT port
   * @return The port
   */
  std::shared_ptr<uint16_t> getMqttPort() const;

  /**
   * Reads the MQTT username
   * @return The username
   */
  std::shared_ptr<std::string> getMqttUsername() const;

  /**
   * Reads the MQTT password
   * @return The password
   */
  std::shared_ptr<std::string> getMqttPassword();
};
