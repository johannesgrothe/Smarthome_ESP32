#pragma once

#include <ArduinoJson.h>
#include <sstream>
#include <utility>
#include <cmath>
#include <string>
#include "../system_settings.h"
#include "../console_logger.h"
#include "../datatypes.h"

#ifndef UNIT_TEST
#include <EEPROM.h>
#else
#include "../test_dummys/dummy_eeprom.h"
#endif

#include "../network_mode.h"
#include "../pin_profile.h"
#include "../gadgets/gadget_characteristic_settings.h"
#include "../status_codes.h"
#include "config.h"
#include "system_storage.h"

// region CONSTANTS

// valid config bitfield
#define VALID_CONFIG_BITFIELD_BYTE 0

#define CONFIG_CHECK_INDEX_ID 0
#define CONFIG_CHECK_INDEX_WIFI_SSID 1
#define CONFIG_CHECK_INDEX_WIFI_PW 2
#define CONFIG_CHECK_INDEX_MQTT_IP 3
#define CONFIG_CHECK_INDEX_MQTT_PORT 4
#define CONFIG_CHECK_INDEX_MQTT_USER 5
#define CONFIG_CHECK_INDEX_MQTT_PW 6

// valid gadget storage bitfield
#define GADGET_COUNT_POS 1

// valid remote storage bitfield
#define SYSTEM_SETTINGS_BITFIELD_BYTE 2

// ir
#define IR_RECV_PIN_POS 3
#define IR_SEND_PIN_POS 4

// radio
#define RADIO_RECV_POS 5
#define RADIO_SEND_POS 6

// network mode
#define NETWORK_MODE_POS 7

// remotes
#define GADGET_REMOTE_POS 8
#define CODE_REMOTE_POS GADGET_REMOTE_POS + 1
#define EVENT_REMOTE_POS CODE_REMOTE_POS + 1

// id
#define ID_POS 15
#define ID_MAX_LEN 20

//wifi_ssid
#define WIFI_SSID_POS (ID_POS + ID_MAX_LEN + 1)
#define WIFI_SSID_MAX_LEN 25

//wifi_pw
#define WIFI_PW_POS (WIFI_SSID_POS + WIFI_SSID_MAX_LEN + 1)
#define WIFI_PW_MAX_LEN 50

//wifi_pw
#define MQTT_IP_POS (WIFI_PW_POS + WIFI_PW_MAX_LEN + 1)
#define MQTT_IP_MAX_LEN 4

//mqtt port
#define MQTT_PORT_POS (MQTT_IP_POS + MQTT_IP_MAX_LEN + 1)
#define MQTT_PORT_MAX_LEN 2

//mqtt username
#define MQTT_USER_POS (MQTT_PORT_POS + MQTT_PORT_MAX_LEN + 1)
#define MQTT_USER_MAX_LEN 50

//mqtt password
#define MQTT_PW_POS (MQTT_USER_POS + MQTT_USER_MAX_LEN + 1)
#define MQTT_PW_MAX_LEN 50

//gadgets
#define GADGET_POS_START (MQTT_PW_POS + MQTT_PW_MAX_LEN + 1)
#define GADGET_MAX_COUNT 8
#define GADGET_BLOCK_START (GADGET_POS_START + ((GADGET_MAX_COUNT + 1) * 2))

#define GADGET_BF_POS 0
#define GADGET_TYPE_POS 1
#define GADGET_PIN_BLOCK_POS 2
#define GADGET_NAME_LEN_POS (GADGET_PIN_BLOCK_POS + GADGET_PIN_BLOCK_LEN + 1)
#define GADGET_JSON_LEN_POS (GADGET_NAME_LEN_POS + 1)
#define GADGET_NAME_POS (GADGET_JSON_LEN_POS + 2)

// endregion

/**
 * System storage class handling EEPROM saving
 */
class EepromStorage: public SystemStorage {
private:

  /**
   * Method takes a bitfield, changes the selected bit to the passed value and returns the new bitfield
   * @param index the bit to be written (0-7)
   * @param new_value the value to be written
   * @param bitfield the bitfield to be changed
   * @return
   */
  static uint8_t calculateNewContentFlag(uint8_t index, bool new_value, uint8_t bitfield);

  /**
   * Extracts the bit at the selected index from the byte
   * @param index index to get the bit from
   * @param bitfield the bitfield to get the bit from
   * @return
   */
  static bool getValueFromContentFlag(uint8_t index, uint8_t bitfield);

  /**
   * Sets the bit of a bitfield to the selected value
   * @param bitfield_address the eeprom-address the bitfield is stored in
   * @param index the bit to be written (0-7)
   * @param value the value to be written
   */
  static void setFlag(int bitfield_address, uint8_t index, bool value);

  /**
   * Reads the value stored at the selected index of a bitfield
   * @param bitfield_address the eeprom-address the bitfield is stored in
   * @param index the bit to be read (0-7)
   * @return the value of the read bit
   */
  static bool getFlag(int bitfield_address, uint8_t index);

  /**
   * Sets the bit at the content bitfield to the selected value
   * @param index the index the byte is located at
   * @param value the value to set it to
   */
  static void setContentFlag(uint8_t index, bool value);

  /**
   * Returns the bit in the content bitfield at the selected index
   * @param index the index the bit is located at
   * @return the value of the bit at the selected index
   */
  static bool getContentFlag(uint8_t index);

  /**
   * Writes a uint8_t (one byte) to the eeprom
   * @param pos position to write to
   * @param value value to be written
   * @return whether writing was successful
   */
  static bool writeUInt8(int pos, uint8_t value);

  /**
   * Reads a uint8_t (one byte) from the selected part of the memory
   * @param pos position to read from
   * @return the uint8_t value
   */
  static uint8_t readUInt8(int pos);

  /**
   * Writes a uint16_t (two bytes) to the selected part of the memory
   * @param pos position to write to
   * @param value the value to write
   * @return whether writing was successful
   */
  static bool writeUInt16(int pos, uint16_t value);

  /**
   * Reads a uint16_t (two bytes) from the selected part of the memory
   * @param pos position to read from
   * @return the uint16_t value
   */
  static uint16_t readUInt16(int pos);

  /**
   * Writes the content of a string to the eeprom
   * @param start start-index for the string
   * @param max_len length of the string
   * @param content the string to write
   * @return whether writing was successful
   */
  static bool writeString(int start, int max_len, std::string content);

  /**
   * Reads the sontent of the eeprom as a string
   * @param start start-index of the string
   * @param max_len maximum length of the string to read
   * @return the read content
   */
  static std::string readString(int start, int max_len);

  /**
   * Reads how many gadgets are currently saved in the eeprom
   * @return the gadget count
   */
  static uint8_t getGadgetCount();

  /**
   * Writes the new gadget count to the eerpom
   * @param count how many gadgets are stored in eeprom
   * @return whether saving was successful or not
   */
  static bool writeGadgetCount(uint8_t count);

  /**
   * Gets the memory position the selected gadget starts at
   * @param gadget_nr the nr of the wanted gadget
   * @return the memory position
   */
  static uint16_t getGadgetMemoryStart(uint8_t gadget_nr);

  /**
   * Gets the end of the memory segment for the selected gadget
   * @param gadget_nr the nr of the wanted gadget
   * @return the end of the memory
   */
  static uint16_t getGadgetMemoryEnd(uint8_t gadget_nr);

  /**
   * Sets the end of the memory segment for the selected gadget
   * @param gadget_nr the nr of the wanted gadget
   * @param mem_end the end of the memory
   * @return whether writing was successful
   */
  static bool setGadgetMemoryEnd(uint8_t gadget_nr, uint16_t mem_end);

  /**
   * Writes the data for a gadget to the eeprom
   * @param config_bf The configuration-bitfield
   * @param gadget_type The type of the gadget
   * @param name The name of the gadget
   * @param gadget_json The general gadget config
   * @param code_json The code-mapping config
   * @return whether writing was successful
   */
  static WriteGadgetStatus writeNewGadget(uint8_t gadget_type, bitfield_set config_bf, pin_set ports, const std::string& name, const std::string& gadget_json, const std::string& code_json);

  /**
   * Returns the index for an specific gadget name or -1 it name was not found.
   * @param name The name you're searching for
   * @return The index of the gadget or -1 if the name wasnt found
   */
  static int getGadgetIndexForName(const std::string& name);

  /**
   * Reads the data for a gadget from the eeprom
   * @param gadget_index the position of the gadget
   * @return the data for the gadget
   */
  static gadget_tuple readGadget(uint8_t gadget_index);


  /**
   * Collects all used ports from all gadgets
   * @return the used ports
   */
  static std::vector<uint8_t> readAllGadgetPorts();

  /**
   * Collects all the names from the gadgets
   * @return the gadget names
   */
  static std::vector<std::string> readAllGadgetNames();


  // read and write gadgets
  /**
   * Reads all gadgets from the eeprom
   * @return a vector containing all gadget information
   */
  static std::vector<gadget_tuple> readAllGadgets();

  /**
   * Writes the data for a gadget to the eeprom
   * @param config_bf the configuration-bitfield
   * @param gadget_type the gadget-type
   * @param name the name of the gadget
   * @param gadget_json the general gadget config
   * @param code_json the code-mapping config
   * @return whether writing was successful
   */
  static WriteGadgetStatus writeGadget(uint8_t gadget_type, bitfield_set config_bf, pin_set ports, const std::string& name, const std::string& gadget_json, const std::string& code_json);

  /**
   * Deletes the gadget with the selected index and moves all other gadgets up
   * @param gadget_index the index of the gadgets to be deleted
   * @return whether the process of deleting was successful
   */
  static bool deleteGadget(uint8_t gadget_index);

  /**
   * Invalidates all saved gadgets
   * @return whether invalidating the gadgets was successful
   */
  static bool resetGadgets();

  // read + write IR pins
  /**
   * Writes the ir receive pin to the eeprom
   * @param pin the pin used for ir receiver
   * @return whether writing was successful
   */
  static bool writeIRrecvPin(uint8_t pin);

  /**
   * Reads the ir receive pin from the eeprom
   * @return the ir recv pin
   */
  static uint8_t readIRrecvPin();

  /**
   * Writes the ir send pin to the eeprom
   * @param pin the pin used for ir sender
   * @return whether writing was successful
   */
  static bool writeIrSendPin(uint8_t pin);

  /**
   * Reads the ir send pin from the eeprom
   * @return the ir send pin
   */
  static uint8_t readIrSendPin();

  // read + write radio pins
  /**
   * Writes the radio receiver pin to the eeprom
   * @param pin the pin used for receiving radio
   * @return whether writing radio was successful
   */
  static bool writeRadioRecvPin(uint8_t pin);

  /**
   * Reads the radio receiver pin from the eeprom
   * @return the radio receiver pin
   */
  static uint8_t readRadioRecvPin();

  /**
  * Writes the radio send pin to the eeprom
  * @param pin the pin used for radio sending
  * @return whether writing was successful
  */
  static bool writeRadioSendPin(uint8_t pin);

  /**
   * Reads the radio send pin from the eeprom
   * @return the radio send pin
   */
  static uint8_t readRadioSendPin();

  // read + write network mode
  /**
   * Writes the network mode to the eeprom
   * @param mode the mode the network should use
   * @return whether writing was successful
   */
  static bool writeNetworkMode(NetworkMode mode);

  /**
   * Reads the network mode from the eeprom
   * @return the network mode
   */
  static NetworkMode readNetworkMode();

  // read + write ID
  /**
   * Writes the chip identifier to the eeprom
   * @param id the chip id to be written
   * @return whether writing was successful
   */
  static bool writeID(const std::string& id);

  /**
   * Reads the chip identifier from the eeprom
   * @return the chip identifier
   */
  static std::string readID();

  /**
   * Checks if the there is a valid ID stored in the EEPROM
   * @return whether there is a valid id
   */
  static bool hasValidID();

  /**
   * Writes the WIFI SSID to the eeprom
   * @param ssid the ssid to be written
   * @return whether writing was successful
   */
  static bool writeWifiSSID(const std::string& ssid);

  /**
   * Reads the WIFI SSID from the eeprom
   * @return the wifi ssid
   */
  static std::string readWifiSSID();

  /**
   * Checks if the there is a valid WIFI SSID stored in the EEPROM
   * @return whether there is a valid ssid
   */
  static bool hasValidWifiSSID();

  /**
   * Writes the WIFI password to the eeprom
   * @param pw the password to be written
   * @return whether writing was successful
   */
  static bool writeWifiPW(const std::string& pw);

  /**
   * Reads the WIFI password from the eeprom
   * @return the wifi password
   */
  static std::string readWifiPW();

  /**
   * Checks if the there is a valid WIFI password stored in the EEPROM
   * @return whether there is a valid password
   */
  static bool hasValidWifiPW();

  /**
   * Writes the MQTT IP-Address to the eeprom.
   * Write 0.0.0.0 to set ip to 'no valid ip'
   * @param ip the ip to be written
   * @return whether writing was successful
   */
  static bool writeMQTTIP(const IPContainer &ip);

  /**
   * Reads the MQTT IP-Address from the eeprom
   * @return the ip
   */
  static IPContainer readMQTTIP();

  /**
   * Checks if the there is a valid MQTT IP-Address stored in the EEPROM
   * @return whether there is a valid ip
   */
  static bool hasValidMQTTIP();

  /**
   * Writes the MQTT port to the eeprom.
   * Write 0 to set port to 'no valid port'
   * @param port the port to be written
   * @return whether writing was successful
   */
  static bool writeMQTTPort(uint16_t port);

  /**
   * Reads the MQTT port from the eeprom
   * @return the port
   */
  static uint16_t readMQTTPort();

  /**
   * Checks if the there is a valid MQTT port stored in the EEPROM
   * @return whether there is a valid port
   */
  static bool hasValidMQTTPort();

  /**
   * Writes the MQTT username to the eeprom
   * @param username the username to be written
   * @return whether writing was successful
   */
  static bool writeMQTTUsername(const std::string &username);

  /**
   * Reads the MQTT username from the eeprom
   * @return the username
   */
  static std::string readMQTTUsername();

  /**
   * Checks if the there is a valid MQTT username stored in the EEPROM
   * @return whether there is a valid username
   */
  static bool hasValidMQTTUsername();

  /**
  * Writes the MQTT password to the eeprom
  * @param pw the password to be written
  * @return whether writing was successful
  */
  static bool writeMQTTPassword(const std::string &pw);

  /**
   * Reads the MQTT password from the eeprom
   * @return the password
   */
  static std::string readMQTTPassword();

  /**
   * Checks if the there is a valid MQTT password stored in the EEPROM
   * @return whether there is a valid password
   */
  static bool hasValidMQTTPassword();

  /**
   * Reads the content of the EEPROM
   * @return the content of the first 500 cells of the EEPROM
   */
  static std::string readWholeEEPROM();

  /**
   * Fills EEPROM with repeating '-_-_-_'-sequences
   */
  static void eraseEeprom();

  /**
   * Resets the valid content flag to 0
   */
  static void resetContentFlag();

  /**
   * Initializes the system EEPROM
   * @return whether the EEPROM was correctly initialized
   */
  static bool initEEPROM();

public:

  EepromStorage();

  /**
   * Prints the whole eeprom layout to the console
   */
  static std::string getEepromLayout();

  /**
   * Returns the address of the last used byte in the eeprom.
   * The maximum eeprom is EEPROM_SIZE
   * @return the address of the last used byte in the eeprom
   */
  static uint16_t getEEPROMUsage();

  /**
   * Loads the system config from the EEPROM
   * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
   */
  std::shared_ptr<Config> loadConfig() override;

  /**
   * Saves the config to the EEPROM
   * @param config The config to write
   * @return Whether saving was successful
   */
  bool saveConfig(Config config) override;

  bool eraseConfig() override;
};
