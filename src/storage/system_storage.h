#pragma once

#ifdef STATIC_CONFIG_ACTIVE
#include "static_storage.h"
#else
#include "eeprom_storage.h"
#endif

#include "../status_codes.h"
#include "../optional.h"

void yolo() {
  auto k = Optional<int>();
  k.has_value();

  auto l = Optional<std::string>("yolokopter");
  l.has_value();
}

/**
 * System storage class handling EEPROM saving
 */
class System_Storage {
public:

  /**
   * Prints the whole eeprom layout to the console
   */
  static void printStorage() {
    #ifndef STATIC_CONFIG_ACTIVE
    EEPROM_Storage::printEEPROMLayout();
    #endif
  }

  /**
   * Resets the valid content flag to 0
   */
  static void reset() {
    #ifndef STATIC_CONFIG_ACTIVE
    EEPROM_Storage::resetContentFlag();
    #endif
  }

  // init eeprom
  /**
   * Initializes the system EEPROM
   * @return whether the EEPROM was correctly initialized
   */
  static bool init() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::initEEPROM();
    #endif
  }

  // read and write gadgets
  /**
   * Reads the data for a gadget from the eeprom
   * @param gadget_index the position of the gadget
   * @return the data for the gadget
   */
  static gadget_tuple readGadget(uint8_t gadget_index) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readGadget(gadget_index);
    #else
    auto all_gadgets = readAllGadgets();
    return all_gadgets[gadget_index];
    #endif
  }

  /**
   * Reads all gadgets from the eeprom
   * @return a vector containing all gadget information
   */
  static std::vector<gadget_tuple> readAllGadgets() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readAllGadgets();
    #else
    return Static_Storage::readAllGadgets();
    #endif
  }

  /**
   * Collects all used ports from all gadgets
   * @return the used ports
   */
  static std::vector<uint8_t> readAllGadgetPorts() {
    auto gadgets = readAllGadgets();
    std::vector<uint8_t> ports;
    for (auto gadget: gadgets) {
      auto buf_ports = std::get<2>(gadget);
      for (auto port: buf_ports) {
        if (port != 0) {
          ports.push_back(port);
        }
      }
    }
    return ports;
  }

  /**
   * Collects all the names from the gadgets
   * @return the gadget names
   */
  static std::vector<std::string> readAllGadgetNames() {
    auto gadgets = readAllGadgets();
    std::vector<std::string> names;
    for (auto gadget: gadgets) {
      auto buf_name = std::get<3>(gadget);
      names.push_back(buf_name);
    }
    return names;
  }

  /**
   * Returns the index for an specific gadget name or -1 it name was not found.
   * @param name The name you're searching for
   * @return The index of the gadget or -1 if the name wasnt found
   */
  static int getGadgetIndexForName(const std::string& name) {
    auto all_gadgets = readAllGadgets();
    for (uint8_t i = 0; i < all_gadgets.size(); i++) {
      gadget_tuple buf_g = all_gadgets[i];
      auto gadget_name = std::get<3>(buf_g);
      if (gadget_name == name) {
        return int(i);
      }
    }
    return -1;
  }

  /**
   * Writes the data for a gadget to the eeprom
   * @param config_bf the configuration-bitfield
   * @param gadget_type the gadget-type
   * @param name the name of the gadget
   * @param gadget_json the general gadget config
   * @param code_json the code-mapping config
   * @return whether writing was successful
   */
  static WriteGadgetStatus writeGadget(uint8_t gadget_type, bitfield_set config_bf, pin_set ports, const std::string& name, const std::string& gadget_json, const std::string& code_json) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeGadget(gadget_type, config_bf, ports, name, gadget_json, code_json);
    #else

    #endif
  }

  /**
   * Deletes the gadget with the selected index and moves all other gadgets up
   * @param gadget_index the index of the gadgets to be deleted
   * @return whether the process of deleting was successful
   */
  static bool deleteGadget(uint8_t gadget_index) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::deleteGadget(gadget_index);
    #else
    return false;
    #endif
  }

  /**
   * Invalidates all saved gadgets
   * @return whether invalidating the gadgets was successful
   */
  static bool resetGadgets() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::resetGadgets();
    #else
    return false;
    #endif
  }

  // read + write IR pins
  /**
   * Writes the ir receive pin to the eeprom
   * @param pin the pin used for ir receiver
   * @return whether writing was successful
   */
  static bool writeIRrecvPin(uint8_t pin) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeIRrecvPin(pin);
    #else
    return false;
    #endif
  }

  /**
   * Reads the ir receive pin from the eeprom
   * @return the ir recv pin
   */
  static uint8_t readIRrecvPin() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readIRrecvPin();
    #else

    #endif
  }

  /**
   * Writes the ir send pin to the eeprom
   * @param pin the pin used for ir sender
   * @return whether writing was successful
   */
  static bool writeIRsendPin(uint8_t pin) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeIRsendPin(pin);
    #else
    return false;
    #endif
  }

  /**
   * Reads the ir send pin from the eeprom
   * @return the ir send pin
   */
  static uint8_t readIRsendPin() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readIRsendPin();
    #else

    #endif
  }

  // read + write radio pins
  /**
   * Writes the radio receiver pin to the eeprom
   * @param pin the pin used for receiving radio
   * @return whether writing radio was successful
   */
  static bool writeRadioRecvPin(uint8_t pin) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeRadioRecvPin(pin);
    #else
    return false;
    #endif
  }

  /**
   * Reads the radio receiver pin from the eeprom
   * @return the radio receiver pin
   */
  static uint8_t readRadioRecvPin() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readRadioRecvPin();
    #else

    #endif
  }

  /**
  * Writes the radio send pin to the eeprom
  * @param pin the pin used for radio sending
  * @return whether writing was successful
  */
  static bool writeRadioSendPin(uint8_t pin) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeRadioSendPin(pin);
    #else
    return false;
    #endif
  }

  /**
   * Reads the radio send pin from the eeprom
   * @return the radio send pin
   */
  static uint8_t readRadioSendPin() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readRadioSendPin();
    #else

    #endif
  }

  // read + write network mode
  /**
   * Writes the network mode to the eeprom
   * @param mode the mode the network should use
   * @return whether writing was successful
   */
  static bool writeNetworkMode(NetworkMode mode) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeNetworkMode(mode);
    #else
    return false;
    #endif
  }

  /**
   * Reads the network mode from the eeprom
   * @return the network mode
   */
  static NetworkMode readNetworkMode() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readNetworkMode();
    #else

    #endif
  }

  // read + write ID
  /**
   * Writes the chip identifier to the eeprom
   * @param id the chip id to be written
   * @return whether writing was successful
   */
  static bool writeID(const std::string& id) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeID(id);
    #else
    return false;
    #endif
  }

  /**
   * Reads the chip identifier from the eeprom
   * @return the chip identifier
   */
  static std::string readID() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readID();
    #else

    #endif
  }

  /**
   * Checks if the there is a valid ID stored in the EEPROM
   * @return whether there is a valid id
   */
  static bool hasValidID() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::hasValidID();
    #else
    return true;
    #endif
  }

  /**
   * Writes the WIFI SSID to the eeprom
   * @param ssid the ssid to be written
   * @return whether writing was successful
   */
  static bool writeWifiSSID(const std::string& ssid) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeWifiSSID(ssid);
    #else
    return false;
    #endif
  }

  /**
   * Reads the WIFI SSID from the eeprom
   * @return the wifi ssid
   */
  static std::string readWifiSSID() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readWifiSSID();
    #else

    #endif
  }

  /**
   * Checks if the there is a valid WIFI SSID stored in the EEPROM
   * @return whether there is a valid ssid
   */
  static bool hasValidWifiSSID() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::hasValidWifiSSID();
    #else
    return true;
    #endif
  }

  /**
   * Writes the WIFI password to the eeprom
   * @param pw the password to be written
   * @return whether writing was successful
   */
  static bool writeWifiPW(const std::string& pw) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeWifiPW(pw);
    #else
    return false;
    #endif
  }

  /**
   * Reads the WIFI password from the eeprom
   * @return the wifi password
   */
  static std::string readWifiPW() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readWifiPW();
    #else

    #endif
  }

  /**
   * Checks if the there is a valid WIFI password stored in the EEPROM
   * @return whether there is a valid password
   */
  static bool hasValidWifiPW() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::hasValidWifiPW();
    #else
    return true;
    #endif
  }

  /**
   * Writes the MQTT IP-Address to the eeprom.
   * Write 0.0.0.0 to set ip to 'no valid ip'
   * @param ip the ip to be written
   * @return whether writing was successful
   */
  static bool writeMQTTIP(const IPAddress& ip) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeMQTTIP(ip);
    #else
    return false;
    #endif
  }

  /**
   * Reads the MQTT IP-Address from the eeprom
   * @return the ip
   */
  static IPAddress readMQTTIP() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readMQTTIP();
    #else

    #endif
  }

  /**
   * Checks if the there is a valid MQTT IP-Address stored in the EEPROM
   * @return whether there is a valid ip
   */
  static bool hasValidMQTTIP() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::hasValidMQTTIP();
    #else
    return true;
    #endif
  }

  /**
   * Writes the MQTT port to the eeprom.
   * Write 0 to set port to 'no valid port'
   * @param port the port to be written
   * @return whether writing was successful
   */
  static bool writeMQTTPort(uint16_t port) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeMQTTPort(port);
    #else
    return false;
    #endif
  }

  /**
   * Reads the MQTT port from the eeprom
   * @return the port
   */
  static uint16_t readMQTTPort() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readMQTTPort();
    #else

    #endif
  }

  /**
   * Checks if the there is a valid MQTT port stored in the EEPROM
   * @return whether there is a valid port
   */
  static bool hasValidMQTTPort() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::hasValidMQTTPort();
    #else
    return true;
    #endif
  }

  /**
   * Writes the MQTT username to the eeprom
   * @param username the username to be written
   * @return whether writing was successful
   */
  static bool writeMQTTUsername(const std::string &username) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeMQTTUsername(username);
    #else
    return false;
    #endif
  }

  /**
   * Reads the MQTT username from the eeprom
   * @return the username
   */
  static std::string readMQTTUsername() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readMQTTUsername();
    #else

    #endif
  }

  /**
   * Checks if the there is a valid MQTT username stored in the EEPROM
   * @return whether there is a valid username
   */
  static bool hasValidMQTTUsername() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::hasValidMQTTUsername();
    #else
    return true;
    #endif
  }

  /**
  * Writes the MQTT password to the eeprom
  * @param pw the password to be written
  * @return whether writing was successful
  */
  static bool writeMQTTPassword(const std::string &pw) {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::writeMQTTPassword(pw);
    #else
    return false;
    #endif
  }

  /**
   * Reads the MQTT password from the eeprom
   * @return the password
   */
  static std::string readMQTTPassword() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readMQTTPassword();
    #else

    #endif
  }

  /**
   * Checks if the there is a valid MQTT password stored in the EEPROM
   * @return whether there is a valid password
   */
  static bool hasValidMQTTPassword() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::hasValidMQTTPassword();
    #else
    return true;
    #endif
  }

  /**
   * Checks the EEPROM for valid wifi ssid + password and mqtt ip + port
   * @return whether all of these four are valid
   */
  static bool hasValidNetworkConfig() {
    return (hasValidWifiSSID() && hasValidWifiPW() && hasValidMQTTIP() && hasValidMQTTPort());
  }

  /**
   * Reads the content of the EEPROM
   * @return the content of the first 500 cells of the EEPROM
   */
  static std::string readWholeEEPROM() {
    #ifndef STATIC_CONFIG_ACTIVE
    return EEPROM_Storage::readWholeEEPROM();
    #else

    #endif
  }

  /**
   * Fills EEPROM with repeating '-_-_-_'-sequences
   */
  static void writeTestEEPROM() {
    #ifndef STATIC_CONFIG_ACTIVE
    EEPROM_Storage::writeTestEEPROM();
    #else

    #endif
  }

  /**
   * Returns the address of the last used byte in the eeprom.
   * The maximum eeprom is EEPROM_SIZE
   * @return the address of the last used byte in the eeprom
   */
  static uint16_t getEEPROMUsage() {
    #ifndef STATIC_CONFIG_ACTIVE
    EEPROM_Storage::getEEPROMUsage();
    #else
    return 0;
    #endif
  }
};
