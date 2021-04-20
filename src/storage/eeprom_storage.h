#pragma once

#include <ArduinoJson.h>
#include <EEPROM.h>
#include <sstream>
#include <utility>
#include <cmath>
#include <string>
#include "../system_settings.h"
#include "../console_logger.h"
#include "../datatypes.h"

#include "../network_library.h"
#include "../pin_profile.h"
#include "../gadgets/gadget_characteristic_settings.h"
#include "../status_codes.h"
#include "config.h"

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

/**
 * System storage class handling EEPROM saving
 */
class EEPROM_Storage {
private:

  /**
   * Method takes a bitfield, changes the selected bit to the passed value and returns the new bitfield
   * @param index the bit to be written (0-7)
   * @param new_value the value to be written
   * @param bitfield the bitfield to be changed
   * @return
   */
  static uint8_t calculateNewContentFlag(uint8_t index, bool new_value, uint8_t bitfield) {
    auto content_flag = bitfield;
    auto mask = (unsigned int) pow(2, index);
    if (new_value) {
      content_flag = content_flag | mask;
    } else {
      mask = 0xFF - mask;
      content_flag = content_flag & mask;
    }
    return content_flag;
  }

  /**
   * Extracts the bit at the selected index from the byte
   * @param index index to get the bit from
   * @param bitfield the bitfield to get the bit from
   * @return
   */
  static bool getValueFromContentFlag(uint8_t index, uint8_t bitfield) {
    auto mask = (unsigned int) pow(2, index);
    uint8_t content_info = bitfield & mask;
    return content_info != 0;
  }

  /**
   * Sets the bit of a bitfield to the selected value
   * @param bitfield_address the eeprom-address the bitfield is stored in
   * @param index the bit to be written (0-7)
   * @param value the value to be written
   */
  static void setFlag(int bitfield_address, uint8_t index, bool value) {
    uint8_t content_flag = EEPROM.readByte(bitfield_address);
    content_flag = calculateNewContentFlag(index, value, content_flag);
    EEPROM.writeByte(bitfield_address, content_flag);
    EEPROM.commit();
  }

  /**
   * Reads the value stored at the selected index of a bitfield
   * @param bitfield_address the eeprom-address the bitfield is stored in
   * @param index the bit to be read (0-7)
   * @return the value of the read bit
   */
  static bool getFlag(int bitfield_address, uint8_t index) {
    uint8_t content_flag = EEPROM.readByte(bitfield_address);
    return getValueFromContentFlag(index, content_flag);
  }

  /**
   * Sets the bit at the content bitfield to the selected value
   * @param index the index the byte is located at
   * @param value the value to set it to
   */
  static void setContentFlag(uint8_t index, bool value) {
    setFlag(VALID_CONFIG_BITFIELD_BYTE, index, value);
  }

  /**
   * Returns the bit in the content bitfield at the selected index
   * @param index the index the bit is located at
   * @return the value of the bit at the selected index
   */
  static bool getContentFlag(uint8_t index) {
    return getFlag(VALID_CONFIG_BITFIELD_BYTE, index);
  }

  /**
   * Writes a uint8_t (one byte) to the eeprom
   * @param pos position to write to
   * @param value value to be written
   * @return whether writing was successful
   */
  static bool writeUInt8(int pos, uint8_t value) {
    EEPROM.writeByte(pos, value);
    EEPROM.commit();
    return true;
  }

  /**
   * Reads a uint8_t (one byte) from the selected part of the memory
   * @param pos position to read from
   * @return the uint8_t value
   */
  static uint8_t readUInt8(int pos) {
    return EEPROM.readByte(pos);
  }

  /**
   * Writes a uint16_t (two bytes) to the selected part of the memory
   * @param pos position to write to
   * @param value the value to write
   * @return whether writing was successful
   */
  static bool writeUInt16(int pos, uint16_t value) {

    uint8_t first = value >> (uint8_t) 8;
    uint8_t second = value & (uint16_t) 0x00ff;

    bool success = writeUInt8(pos, first);
    success = success & writeUInt8(pos + 1, second);

    return success;
  }

  /**
   * Reads a uint16_t (two bytes) from the selected part of the memory
   * @param pos position to read from
   * @return the uint16_t value
   */
  static uint16_t readUInt16(int pos) {
    auto first = readUInt8(pos);
    auto second = readUInt8(pos + 1);
    return (uint16_t) ((first * (0xFF + 1)) + second);
  }

  /**
   * Writes the content of a string to the eeprom
   * @param start start-index for the string
   * @param max_len length of the string
   * @param content the string to write
   * @return whether writing was successful
   */
  static bool writeString(int start, int max_len, std::string content) {
    int id_length = content.size();
    if (id_length > max_len) {
      logger.println(LOG_TYPE::ERR, "written content is too long");
      return false;
    }
    for (int pos = 0; pos < id_length; pos++) {
      EEPROM.writeChar(pos + start, content[pos]);
    }
    EEPROM.writeChar(id_length + start, '\0');
    EEPROM.commit();
    return true;
  }

  /**
   * Reads the sontent of the eeprom as a string
   * @param start start-index of the string
   * @param max_len maximum length of the string to read
   * @return the read content
   */
  static std::string readString(int start, int max_len) {
    std::stringstream ss;
    for (int pos = 0; pos < max_len; pos++) {
      char c = EEPROM.readChar(pos + start);
      if (c == '\n' || c == 0) {
        return ss.str();
      }
      ss << c;
    }
    return ss.str();
  }

  /**
   * Reads how many gadgets are currently saved in the eeprom
   * @return the gadget count
   */
  static uint8_t getGadgetCount() {
    return readUInt8(GADGET_COUNT_POS);
  }

  /**
   * Writes the new gadget count to the eerpom
   * @param count how many gadgets are stored in eeprom
   * @return whether saving was successful or not
   */
  static bool writeGadgetCount(uint8_t count) {
    return writeUInt8(GADGET_COUNT_POS, count);
  }

  /**
   * Gets the memory position the selected gadget starts at
   * @param gadget_nr the nr of the wanted gadget
   * @return the memory position
   */
  static uint16_t getGadgetMemoryStart(uint8_t gadget_nr) {
    if (gadget_nr >= GADGET_MAX_COUNT) {
      return 0;
    }
    if (gadget_nr == 0) {
      return GADGET_BLOCK_START;
    }
    return readUInt16(GADGET_POS_START + (gadget_nr * 2));
  }

  /**
   * Gets the end of the memory segment for the selected gadget
   * @param gadget_nr the nr of the wanted gadget
   * @return the end of the memory
   */
  static uint16_t getGadgetMemoryEnd(uint8_t gadget_nr) {
    if (gadget_nr >= GADGET_MAX_COUNT) {
      return 0;
    }
    return readUInt16(GADGET_POS_START + ((gadget_nr + 1) * 2));
  }

  /**
   * Sets the end of the memory segment for the selected gadget
   * @param gadget_nr the nr of the wanted gadget
   * @param mem_end the end of the memory
   * @return whether writing was successful
   */
  static bool setGadgetMemoryEnd(uint8_t gadget_nr, uint16_t mem_end) {
    if (gadget_nr >= GADGET_MAX_COUNT) {
      return false;
    }
    if (mem_end < GADGET_BLOCK_START) {
      return false;
    }
    if (mem_end >= EEPROM_SIZE) {
      return false;
    }
    return writeUInt16(GADGET_POS_START + ((gadget_nr + 1) * 2), mem_end + 1);
  }

  /**
   * Writes the data for a gadget to the eeprom
   * @param config_bf The configuration-bitfield
   * @param gadget_type The type of the gadget
   * @param name The name of the gadget
   * @param gadget_json The general gadget config
   * @param code_json The code-mapping config
   * @return whether writing was successful
   */
  static WriteGadgetStatus writeNewGadget(uint8_t gadget_type, bitfield_set config_bf, pin_set ports, const std::string& name, const std::string& gadget_json, const std::string& code_json) {

    // Check if updating gadget is possible
    uint8_t gadget_index = getGadgetCount();

    // Check if maximum gadget count is reached
    if (gadget_index >= GADGET_MAX_COUNT) {
      logger.println(LOG_TYPE::ERR, "Cannot save gadget: maximum count of gadgets reached");
      return WriteGadgetStatus::MaxGadgetCountReached;
    }

    // Check if name exist and quit if name is already taken
    auto existing_names = EEPROM_Storage::readAllGadgetNames();
    for (const auto& list_name: existing_names) {
      if (name == list_name) {
        logger.printfln(LOG_TYPE::ERR, "Cannot save gadget: gadget name '%s' is already in use", name.c_str());
        return WriteGadgetStatus::NameAlreadyInUse;
      }
    }

    auto existing_ports = EEPROM_Storage::readAllGadgetPorts();

    for (auto gadget_port: ports) {
      // Check if port is configured on the system
      auto buf_pin = getPinForPort(gadget_port);
      if (!buf_pin && gadget_port) {
        logger.printfln(LOG_TYPE::ERR, "Cannot save gadget: port %d is not configured on this system", gadget_port);
        return WriteGadgetStatus::PortNotConfigured;
      }

      // Check if port is already in use on the system
      for (auto existing_port: existing_ports) {
        if (gadget_port == existing_port) {
          logger.printfln(LOG_TYPE::ERR, "Cannot save gadget: gadget tries to use port already occupied (%d)", gadget_port);
          return WriteGadgetStatus::PortAlreadyInUse;
        }
      }
    }

    // Addrees the gadget start at
    uint16_t g_start_addr = getGadgetMemoryStart(gadget_index);

    // Length of the gadget name
    uint8_t g_name_len = name.length();
    // Length of the gadget config json
    uint16_t g_config_len = gadget_json.length();
    // Length of the gadget code json
    uint16_t g_code_len = code_json.length();

    // Position the gadget name starts at
    uint16_t name_start = g_start_addr + GADGET_NAME_POS;
    // Position the gadget config json starts at
    uint16_t config_start = name_start + g_name_len + 1;
    // Position the gadget code json starts at
    uint16_t code_start = config_start + g_config_len + 1;

    // Complete length of the gadget config
    uint16_t complete_len = (code_start + g_code_len + 1) - g_start_addr;
    // Last storage index of the gadget
    uint16_t end_index = g_start_addr + complete_len;

    if (end_index > EEPROM_SIZE) {
      logger.println(LOG_TYPE::ERR, "Cannot save gadget: missing space in eeprom");
      return WriteGadgetStatus::MissingEEPROMSpace;
    }

    // Create the bitfield
    uint8_t buf_bitfield = 0;
    for (uint8_t i = 0; i < 8; i++) {
      buf_bitfield = calculateNewContentFlag(i, config_bf[i], buf_bitfield);
    }

    // write the config bitfield
    auto success = writeUInt8(g_start_addr + GADGET_BF_POS, buf_bitfield);

    // Write the gadget type
    success = success && writeUInt8(g_start_addr + GADGET_TYPE_POS, gadget_type);

    for (uint8_t i = 0; i < GADGET_PIN_BLOCK_LEN; i++) {
      // Write the port config at position i
      success = success && writeUInt8(g_start_addr + GADGET_PIN_BLOCK_POS + i, ports[i]);
    }

    // Write the length of the name
    success = success && writeUInt8(g_start_addr + GADGET_NAME_LEN_POS, g_name_len);
    // Write the length of the config length
    success = success && writeUInt16(g_start_addr + GADGET_JSON_LEN_POS, g_config_len);
    // Write the name
    success = success && writeString(name_start, g_name_len, name);
    // Write the config json
    success = success && writeString(config_start, g_config_len, gadget_json);
    // Write the code json
    success = success && writeString(code_start, g_code_len, code_json);

    if (!success) {
      logger.println(LOG_TYPE::ERR, "Cannot save gadget: error writing content");
      return WriteGadgetStatus::ErrorWritingContent;
    }

    // set the starting point for the next gadget
    if (!setGadgetMemoryEnd(gadget_index, end_index)) {
      logger.println(LOG_TYPE::ERR, "Cannot save gadget: error saving gadget memory end");
      return WriteGadgetStatus::ErrorSavingMemoryEnd;
    }

    // increment gadget count to formally "save" the gadget
    writeGadgetCount(gadget_index + 1);
    return WriteGadgetStatus::WritingOK;
  }

  /**
   * Returns the index for an specific gadget name or -1 it name was not found.
   * @param name The name you're searching for
   * @return The index of the gadget or -1 if the name wasnt found
   */
  static int getGadgetIndexForName(const std::string& name) {
    auto gadget_count = getGadgetCount();
    for (uint8_t i = 0; i < gadget_count; i++) {
      gadget_tuple buf_g = readGadget(i);
      auto gadget_name = std::get<3>(buf_g);
      if (gadget_name == name) {
        return int(i);
      }
    }
    return -1;
  }

  /**
   * Reads the data for a gadget from the eeprom
   * @param gadget_index the position of the gadget
   * @return the data for the gadget
   */
  static gadget_tuple readGadget(uint8_t gadget_index) {
    auto addr = getGadgetMemoryStart(gadget_index);
    auto addr_end = getGadgetMemoryEnd(gadget_index);
    auto stored_gadgets = readUInt8(GADGET_COUNT_POS);

    pin_set pins = {0, 0, 0, 0, 0};
    bitfield_set remote_bf = {false, false, false, false, false, false, false, false};

    if (!addr || !addr_end || gadget_index >= stored_gadgets) {
      return gadget_tuple(0, remote_bf, pins, "", "", "");
    }

    auto config_bf = readUInt8(addr + GADGET_BF_POS);
    auto gadget_type = readUInt8(addr + GADGET_TYPE_POS);
    auto gadget_name_len = readUInt8(addr + GADGET_NAME_LEN_POS);
    auto gadget_json_len = readUInt16(addr + GADGET_JSON_LEN_POS);

    uint16_t name_start = addr + GADGET_NAME_POS;
    uint16_t config_start = name_start + gadget_name_len + 1;
    uint16_t code_start = config_start + gadget_json_len + 1;

    auto gadget_name = readString(name_start, gadget_name_len);
    auto gadget_json = readString(config_start, gadget_json_len);
    auto code_json = readString(code_start, addr_end);

    for (uint8_t i = 0; i < GADGET_PIN_BLOCK_LEN; i++) {
      pins[i] = readUInt8(addr + GADGET_PIN_BLOCK_POS + i);
    }

    for (uint8_t i = 0; i < 8; i++) {
      remote_bf[i] = getValueFromContentFlag(i, config_bf);
    }

    return gadget_tuple(gadget_type, remote_bf, pins, gadget_name, gadget_json, code_json);
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


  // read and write gadgets
  /**
   * Reads all gadgets from the eeprom
   * @return a vector containing all gadget information
   */
  static std::vector<gadget_tuple> readAllGadgets() {
    auto gadget_count = getGadgetCount();
    std::vector<gadget_tuple> gadgets;
    for (uint8_t i = 0; i < gadget_count; i++) {
      gadget_tuple buf_g = readGadget(i);
      gadgets.push_back(buf_g);
    }
    return gadgets;
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

    if (gadget_type >= GadgetIdentifierCount) {
      logger.printfln(LOG_TYPE::ERR, "Unknown gadget identifier '%d'", gadget_type);
      return WriteGadgetStatus::GadgetTypeError0;
    }

    auto type = (GadgetIdentifier) gadget_type;

    if (type == GadgetIdentifier::err_type) {
      logger.println(LOG_TYPE::ERR, "Cannot save gadget: gadget has err-type 0");
      return WriteGadgetStatus::GadgetTypeErrorUnknown;
    } else {
      logger.printfln("Saving gadget '%s' with type %d", name.c_str(), gadget_type);
    }

    DynamicJsonDocument buf_doc(2000);

    // Check gadget config
    if (!gadget_json.empty()) {
      auto err = deserializeJson(buf_doc, gadget_json);
      if (err != DeserializationError::Ok) {
        logger.printfln(LOG_TYPE::ERR, "Cannot save gadget: received faulty gadget config");
        return WriteGadgetStatus::FaultyConfigJSON;
      }
    }

    // Check code config
    if (!code_json.empty()) {
      auto err = deserializeJson(buf_doc, code_json);
      if (err != DeserializationError::Ok) {
        logger.printfln(LOG_TYPE::ERR, "Cannot save gadget: received faulty code config");
        return WriteGadgetStatus::FaultyCodeConfig;
      }
    }

    // Check if updating gadget is possible
    int index = getGadgetIndexForName(name);

    if (index != -1) {
      if (!(deleteGadget(index))) {
        return WriteGadgetStatus::DeletionFailed;
      }
    }

    return writeNewGadget(gadget_type, config_bf, ports, name, gadget_json, code_json);
  }

  /**
   * Deletes the gadget with the selected index and moves all other gadgets up
   * @param gadget_index the index of the gadgets to be deleted
   * @return whether the process of deleting was successful
   */
  static bool deleteGadget(uint8_t gadget_index) {
    auto gadget_count = getGadgetCount();

    if (gadget_count == 0) {
      logger.println(LOG_TYPE::ERR, "Cannot delete gadget: no gadget saved");
      return false;
    }

    if (gadget_index > gadget_count -1) {
      logger.println(LOG_TYPE::ERR, "Cannot delete gadget: index does not exist");
      return false;
    }

    if (gadget_index == gadget_count -1) {
      return writeGadgetCount(gadget_count - 1);
    }

    std::vector<gadget_tuple> gadgets;

    for (uint8_t i = gadget_index + 1; i < gadget_count; i++) {
      auto buf_gadget = readGadget(i);
      gadgets.push_back(buf_gadget);
    }

    if (!writeGadgetCount(gadget_index)) {
      logger.println(LOG_TYPE::ERR, "Error in deleting process while updating gadget count");
      return false;
    }

    auto copy_gadget_count = (uint8_t) gadgets.size();

    for (uint8_t i = 0; i < copy_gadget_count; i++) {
      auto buf_gadget = gadgets[i];

      auto e1 = std::get<0>(buf_gadget);
      auto e2 = std::get<1>(buf_gadget);
      auto e3 = std::get<2>(buf_gadget);
      auto e4 = std::get<3>(buf_gadget);
      auto e5 = std::get<4>(buf_gadget);
      auto e6 = std::get<5>(buf_gadget);

      auto status = writeGadget(e1, e2, e3, e4, e5, e6);

      if (status != WriteGadgetStatus::WritingOK) {
        logger.println(LOG_TYPE::ERR, "Error in in deletion process: moving gadgets failed");
        return false;
      }
    }
    return true;
  }

  /**
   * Invalidates all saved gadgets
   * @return whether invalidating the gadgets was successful
   */
  static bool resetGadgets() {
    return writeGadgetCount(0);
  }

  // read + write IR pins
  /**
   * Writes the ir receive pin to the eeprom
   * @param pin the pin used for ir receiver
   * @return whether writing was successful
   */
  static bool writeIRrecvPin(uint8_t pin) {
    return writeUInt8(IR_RECV_PIN_POS, pin);
  }

  /**
   * Reads the ir receive pin from the eeprom
   * @return the ir recv pin
   */
  static uint8_t readIRrecvPin() {
    return readUInt8(IR_RECV_PIN_POS);
  }

  /**
   * Writes the ir send pin to the eeprom
   * @param pin the pin used for ir sender
   * @return whether writing was successful
   */
  static bool writeIRsendPin(uint8_t pin) {
    return writeUInt8(IR_SEND_PIN_POS, pin);
  }

  /**
   * Reads the ir send pin from the eeprom
   * @return the ir send pin
   */
  static uint8_t readIRsendPin() {
    return readUInt8(IR_SEND_PIN_POS);
  }

  // read + write radio pins
  /**
   * Writes the radio receiver pin to the eeprom
   * @param pin the pin used for receiving radio
   * @return whether writing radio was successful
   */
  static bool writeRadioRecvPin(uint8_t pin) {
    return writeUInt8(RADIO_RECV_POS, pin);
  }

  /**
   * Reads the radio receiver pin from the eeprom
   * @return the radio receiver pin
   */
  static uint8_t readRadioRecvPin() {
    return readUInt8(RADIO_RECV_POS);
  }

  /**
  * Writes the radio send pin to the eeprom
  * @param pin the pin used for radio sending
  * @return whether writing was successful
  */
  static bool writeRadioSendPin(uint8_t pin) {
    return writeUInt8(RADIO_SEND_POS, pin);
  }

  /**
   * Reads the radio send pin from the eeprom
   * @return the radio send pin
   */
  static uint8_t readRadioSendPin() {
    return readUInt8(RADIO_SEND_POS);
  }

  // read + write network mode
  /**
   * Writes the network mode to the eeprom
   * @param mode the mode the network should use
   * @return whether writing was successful
   */
  static bool writeNetworkMode(NetworkMode mode) {
    return writeUInt8(NETWORK_MODE_POS, (uint8_t) mode);
  }

  /**
   * Reads the network mode from the eeprom
   * @return the network mode
   */
  static NetworkMode readNetworkMode() {
    uint8_t mode = readUInt8(NETWORK_MODE_POS);
    if (mode < NetworkModeCount) {
      return (NetworkMode) mode;
    }
    return NetworkMode::None;
  }

  // read + write ID
  /**
   * Writes the chip identifier to the eeprom
   * @param id the chip id to be written
   * @return whether writing was successful
   */
  static bool writeID(const std::string& id) {
    bool success;
    if (id == "") {
      setContentFlag(CONFIG_CHECK_INDEX_ID, false);
      success = true;
    } else {
      success = writeString(ID_POS, ID_MAX_LEN, id);
      setContentFlag(CONFIG_CHECK_INDEX_ID, success);
    }
    return success;
  }

  /**
   * Reads the chip identifier from the eeprom
   * @return the chip identifier
   */
  static std::string readID() {
    if (!hasValidID()) {
      std::stringstream sstr;
      sstr << "esp_" << (micros() % 17731776);
      writeID(sstr.str());
    }
    return readString(ID_POS, ID_MAX_LEN);
  }

  /**
   * Checks if the there is a valid ID stored in the EEPROM
   * @return whether there is a valid id
   */
  static bool hasValidID() {
    return getContentFlag(CONFIG_CHECK_INDEX_ID);
  }

  /**
   * Writes the WIFI SSID to the eeprom
   * @param ssid the ssid to be written
   * @return whether writing was successful
   */
  static bool writeWifiSSID(const std::string& ssid) {
    bool success;
    if (ssid == "") {
      setContentFlag(CONFIG_CHECK_INDEX_WIFI_SSID, false);
      success = true;
    } else {
      success = writeString(WIFI_SSID_POS, WIFI_SSID_MAX_LEN, ssid);
      setContentFlag(CONFIG_CHECK_INDEX_WIFI_SSID, success);
    }
    return success;
  }

  /**
   * Reads the WIFI SSID from the eeprom
   * @return the wifi ssid
   */
  static std::string readWifiSSID() {
    return readString(WIFI_SSID_POS, WIFI_SSID_MAX_LEN);
  }

  /**
   * Checks if the there is a valid WIFI SSID stored in the EEPROM
   * @return whether there is a valid ssid
   */
  static bool hasValidWifiSSID() {
    return getContentFlag(CONFIG_CHECK_INDEX_WIFI_SSID);
  }

  /**
   * Writes the WIFI password to the eeprom
   * @param pw the password to be written
   * @return whether writing was successful
   */
  static bool writeWifiPW(const std::string& pw) {
    bool success;
    if (pw == "") {
      setContentFlag(CONFIG_CHECK_INDEX_WIFI_PW, false);
      success = true;
    } else {
      success = writeString(WIFI_PW_POS, WIFI_PW_MAX_LEN, pw);
      setContentFlag(CONFIG_CHECK_INDEX_WIFI_PW, success);
    }
    return success;
  }

  /**
   * Reads the WIFI password from the eeprom
   * @return the wifi password
   */
  static std::string readWifiPW() {
    return readString(WIFI_PW_POS, WIFI_PW_MAX_LEN);
  }

  /**
   * Checks if the there is a valid WIFI password stored in the EEPROM
   * @return whether there is a valid password
   */
  static bool hasValidWifiPW() {
    return getContentFlag(CONFIG_CHECK_INDEX_WIFI_PW);
  }

  /**
   * Writes the MQTT IP-Address to the eeprom.
   * Write 0.0.0.0 to set ip to 'no valid ip'
   * @param ip the ip to be written
   * @return whether writing was successful
   */
  static bool writeMQTTIP(const IPAddress& ip) {
    bool success = true;
    for (int i = 0; i < 4; i++) {
      success = success && writeUInt8(MQTT_IP_POS + i, ip[i]);
    }
    if (ip == IPAddress(0, 0, 0, 0)) {
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_IP, false);
      success = true;
    } else {
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_IP, success);
    }
    return success;
  }

  /**
   * Reads the MQTT IP-Address from the eeprom
   * @return the ip
   */
  static IPAddress readMQTTIP() {
    IPAddress ip;
    for (int i = 0; i < 4; i++) {
      ip[i] = readUInt8(MQTT_IP_POS + i);
    }
    return ip;
  }

  /**
   * Checks if the there is a valid MQTT IP-Address stored in the EEPROM
   * @return whether there is a valid ip
   */
  static bool hasValidMQTTIP() {
    return getContentFlag(CONFIG_CHECK_INDEX_MQTT_IP);
  }

  /**
   * Writes the MQTT port to the eeprom.
   * Write 0 to set port to 'no valid port'
   * @param port the port to be written
   * @return whether writing was successful
   */
  static bool writeMQTTPort(uint16_t port) {
    bool success = writeUInt16(MQTT_PORT_POS, port);

    if (port == 0) {
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_PORT, false);
      success = true;
    } else {
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_PORT, success);
    }
    return success;
  }

  /**
   * Reads the MQTT port from the eeprom
   * @return the port
   */
  static uint16_t readMQTTPort() {
    return readUInt16(MQTT_PORT_POS);
  }

  /**
   * Checks if the there is a valid MQTT port stored in the EEPROM
   * @return whether there is a valid port
   */
  static bool hasValidMQTTPort() {
    return getContentFlag(CONFIG_CHECK_INDEX_MQTT_PORT);
  }

  /**
   * Writes the MQTT username to the eeprom
   * @param username the username to be written
   * @return whether writing was successful
   */
  static bool writeMQTTUsername(const std::string &username) {
    bool success;
    success = writeString(MQTT_USER_POS, MQTT_USER_MAX_LEN, username);
    if (username == "") {
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_USER, false);
      success = true;
    } else {
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_USER, success);
    }
    return success;
  }

  /**
   * Reads the MQTT username from the eeprom
   * @return the username
   */
  static std::string readMQTTUsername() {
    return readString(MQTT_USER_POS, MQTT_USER_MAX_LEN);
  }

  /**
   * Checks if the there is a valid MQTT username stored in the EEPROM
   * @return whether there is a valid username
   */
  static bool hasValidMQTTUsername() {
    return getContentFlag(CONFIG_CHECK_INDEX_MQTT_USER);
  }

  /**
  * Writes the MQTT password to the eeprom
  * @param pw the password to be written
  * @return whether writing was successful
  */
  static bool writeMQTTPassword(const std::string &pw) {
    bool success;
    success = writeString(MQTT_PW_POS, MQTT_PW_MAX_LEN, pw);
    if (pw == "") {
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_PW, false);
      success = true;
    } else {
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_PW, success);
    }
    return success;
  }

  /**
   * Reads the MQTT password from the eeprom
   * @return the password
   */
  static std::string readMQTTPassword() {
    return readString(MQTT_PW_POS, MQTT_PW_MAX_LEN);
  }

  /**
   * Checks if the there is a valid MQTT password stored in the EEPROM
   * @return whether there is a valid password
   */
  static bool hasValidMQTTPassword() {
    return getContentFlag(CONFIG_CHECK_INDEX_MQTT_PW);
  }

  /**
   * Reads the content of the EEPROM
   * @return the content of the first 500 cells of the EEPROM
   */
  static std::string readWholeEEPROM() {
    std::stringstream ss;
    for (int i = 0; i < 500; i++) {
      char bufchar = EEPROM.readChar(i);
      if (bufchar == 0) {
        ss << "\\n";
      } else {
        ss << bufchar;
      }
    }
    return ss.str();
  }

  /**
   * Fills EEPROM with repeating '-_-_-_'-sequences
   */
  static void writeTestEEPROM() {
    for (int i = 0; i < 750; i++) {
      if (i % 2) {
        EEPROM.writeChar(i, '-');
      } else {
        EEPROM.writeChar(i, '_');
      }
//      char k = (char) (48 + (i % 10));
//      EEPROM.writeChar(i, k);
    }

    writeUInt8(GADGET_COUNT_POS, 0);
    writeUInt8(VALID_CONFIG_BITFIELD_BYTE, 0);
    writeUInt8(SYSTEM_SETTINGS_BITFIELD_BYTE, 0);

    EEPROM.commit();
  }

  /**
 * Resets the valid content flag to 0
 */
  static void resetContentFlag() {
    uint8_t content_flag = 0;
    EEPROM.writeByte(VALID_CONFIG_BITFIELD_BYTE, content_flag);
    EEPROM.commit();
  }

  /**
 * Writes the value of a param to the eeprom
 * @param param_name Name of the param to write
 * @param param_val Value of the param as string
 * @param param_val_uint Value of the param as uint
 * @return whether writing was successful
 */
  bool writeConfigParam(const std::string &param_name, const std::string &param_val, uint8_t param_val_uint) {
    logger.printfln("Write param '%s': ", param_name.c_str());
    bool write_successful = false;

    // write ID
    if (param_name == "id") {
      write_successful = EEPROM_Storage::writeID(param_val);
    }

      // Write Wifi SSID
    else if (param_name == "wifi_ssid") {
      write_successful = EEPROM_Storage::writeWifiSSID(param_val);
    }

      // Write Wifi PW
    else if (param_name == "wifi_pw") {
      write_successful = EEPROM_Storage::writeWifiPW(param_val);
    }

      // Write MQTT IP
    else if (param_name == "mqtt_ip") {
      if (param_val == "") {
        write_successful = EEPROM_Storage::writeMQTTIP(IPAddress(0, 0, 0, 0));
      } else {
        IPAddress buf_ip;
        buf_ip.fromString(param_val.c_str());
        write_successful = EEPROM_Storage::writeMQTTIP(buf_ip);
      }
    }

      // Write MQTT Port
    else if (param_name == "mqtt_port") {
      write_successful = EEPROM_Storage::writeMQTTPort((uint16_t) atoi(param_val.c_str()));
    }

      // Write MQTT User
    else if (param_name == "mqtt_user") {
      write_successful = EEPROM_Storage::writeMQTTUsername(param_val);
    }

      // Write MQTT PW
    else if (param_name == "mqtt_pw") {
      write_successful = EEPROM_Storage::writeMQTTPassword(param_val);
    }

      // Write IR recv
    else if (param_name == "irrecv_pin") {
      write_successful = EEPROM_Storage::writeIRrecvPin(param_val_uint);
    }

      // Write IR send
    else if (param_name == "irsend_pin") {
      write_successful = EEPROM_Storage::writeIRsendPin(param_val_uint);
    }

      // Write radio receiver pin
    else if (param_name == "radio_recv_pin") {
      write_successful = EEPROM_Storage::writeRadioRecvPin(param_val_uint);
    }

      // Write radio sender pin
    else if (param_name == "radio_send_pin") {
      write_successful = EEPROM_Storage::writeRadioSendPin(param_val_uint);
    }

      // Write network mode
    else if (param_name == "network_mode") {
      if (param_val_uint < NetworkModeCount) {
        write_successful = EEPROM_Storage::writeNetworkMode((NetworkMode) param_val_uint);
      } else {
        write_successful = false;
      }
    }

    if (write_successful) {
      logger.println("OK");
    } else {
      logger.println(LOG_TYPE::ERR, "Failed");
    }

    return write_successful;
  }


public:

  // init eeprom
  /**
   * Initializes the system EEPROM
   * @return whether the EEPROM was correctly initialized
   */
  static bool initEEPROM() {
    logger.println("Initializing EEPROM...");

    if (!EEPROM.begin(EEPROM_SIZE)) {
      logger.println(LOG_TYPE::ERR, "failed to initialize EEPROM");
      return false;
    }
    return true;
  }

  /**
   * Prints the whole eeprom layout to the console
   */
  static void printEEPROMLayout() {
    std::stringstream ss;
    ss << "EEPROM config:";
    ss << "\nvalid config bitfield: " << VALID_CONFIG_BITFIELD_BYTE;
    ss << "\ngadget count: " << GADGET_COUNT_POS;
    ss << "\nsystem settings bitfield: " << SYSTEM_SETTINGS_BITFIELD_BYTE;
    ss << "\nir_recv pin: " << IR_RECV_PIN_POS;
    ss << "\nir_send pin: " << IR_SEND_PIN_POS;
    ss << "\nradio_recv pin: " << RADIO_RECV_POS;
    ss << "\nradio_send pin: " << RADIO_SEND_POS;
    ss << "\nnetwork mode: " << NETWORK_MODE_POS;
    ss << "\ngadget remote: " << GADGET_REMOTE_POS;
    ss << "\ncode remote: " << CODE_REMOTE_POS;
    ss << "\nevent remote: " << EVENT_REMOTE_POS;
    ss << "\nid: " << ID_POS << " - " << ID_POS + ID_MAX_LEN;
    ss << "\nwifi_ssid: " << WIFI_SSID_POS << " - " << WIFI_SSID_POS + WIFI_SSID_MAX_LEN;
    ss << "\nwifi_pw: " << WIFI_PW_POS << " - " << WIFI_PW_POS + WIFI_PW_MAX_LEN;
    ss << "\nmqtt_ip: " << MQTT_IP_POS << " - " << MQTT_IP_POS + MQTT_IP_MAX_LEN;
    ss << "\nmqtt_port: " << MQTT_PORT_POS << " - " << MQTT_PORT_POS + MQTT_PORT_MAX_LEN;
    ss << "\nmqtt_user: " << MQTT_USER_POS << " - " << MQTT_USER_POS + MQTT_USER_MAX_LEN;
    ss << "\nmqtt_pw: " << MQTT_PW_POS << " - " << MQTT_PW_POS + MQTT_PW_MAX_LEN;
    Serial.println(ss.str().c_str());
  }

  /**
   * Returns the address of the last used byte in the eeprom.
   * The maximum eeprom is EEPROM_SIZE
   * @return the address of the last used byte in the eeprom
   */
  static uint16_t getEEPROMUsage() {
    return getGadgetMemoryEnd(getGadgetCount() - 1);
  }

  /**
   * Loads the system config from the EEPROM
   * @return The loaded Config as shared_ptr, nullptr if config could not be loaded
   */
  static std::shared_ptr<Config> loadConfig() {
    std::string id = EEPROM_Storage::readID();
    NetworkMode network_mode = EEPROM_Storage::readNetworkMode();
    auto gadgets = EEPROM_Storage::readAllGadgets();

    uint8_t ir_recv = EEPROM_Storage::readIRrecvPin();
    uint8_t ir_send = EEPROM_Storage::readIRsendPin();

    uint8_t radio_recv = EEPROM_Storage::readRadioRecvPin();
    uint8_t radio_send = EEPROM_Storage::readRadioSendPin();

    std::shared_ptr <std::string> wifi_ssid = nullptr;
    std::shared_ptr <std::string> wifi_pw = nullptr;

    std::shared_ptr <IPAddress> mqtt_ip = nullptr;
    std::shared_ptr <uint16_t> mqtt_port = nullptr;

    std::shared_ptr <std::string> mqtt_username = nullptr;
    std::shared_ptr <std::string> mqtt_pw = nullptr;

    if (EEPROM_Storage::hasValidWifiSSID()) {
      wifi_ssid = std::make_shared<std::string>(EEPROM_Storage::readWifiSSID());
    }

    if (EEPROM_Storage::hasValidWifiPW()) {
      wifi_pw = std::make_shared<std::string>(EEPROM_Storage::readWifiPW());
    }

    if (EEPROM_Storage::hasValidMQTTIP()) {
      mqtt_ip = std::make_shared<IPAddress>(EEPROM_Storage::readMQTTIP());
    }

    if (EEPROM_Storage::hasValidMQTTPort()) {
      mqtt_port = std::make_shared<uint16_t>(EEPROM_Storage::readMQTTPort());
    }

    if (EEPROM_Storage::hasValidMQTTUsername()) {
      mqtt_username = std::make_shared<std::string>(EEPROM_Storage::readMQTTUsername());
    }

    if (EEPROM_Storage::hasValidMQTTPassword()) {
      mqtt_pw = std::make_shared<std::string>(EEPROM_Storage::readMQTTPassword());
    }

    auto cfg = Config(
        id,
        network_mode,
        gadgets,
        ir_recv,
        ir_send,
        radio_recv,
        radio_send,
        wifi_ssid,
        wifi_pw,
        mqtt_ip,
        mqtt_port,
        mqtt_username,
        mqtt_pw);

    return std::make_shared<Config>(cfg);
  }

  /**
   * Saves the config to the EEPROM
   * @param config The config to write
   * @return Whether saving was successful
   */
  static bool writeConfig(Config config) {

    // Write ID
    bool write_successful = writeID(config.getID());

    // Write Network Mode
    write_successful &= writeNetworkMode(config.getNetworkMode());

    // Write Pins
    write_successful &= writeIRrecvPin(config.getIRRecvPin());
    write_successful &= writeIRsendPin(config.getIRSendPin());
    write_successful &= writeRadioRecvPin(config.getRadioRecvPin());
    write_successful &= writeRadioSendPin(config.getRadioSendPin());

    // Write Wifi SSID
    if (config.getWifiSSID() != nullptr && ((hasValidWifiSSID() && *config.getWifiSSID() != readWifiSSID()) || !hasValidWifiSSID())) {
      write_successful &= writeWifiSSID(*config.getWifiSSID());
    } else if (config.getWifiSSID() == nullptr && hasValidWifiSSID()) {
      write_successful &= writeWifiSSID("");
    }

    // Write Wifi PW
    if (config.getWifiPW() != nullptr && ((hasValidWifiPW() && *config.getWifiPW() != readWifiPW()) || !hasValidWifiPW())) {
      write_successful &= writeWifiPW(*config.getWifiPW());
    } else if (config.getWifiPW() == nullptr && hasValidWifiPW()) {
      write_successful &= writeWifiPW("");
    }

    // Write MQTT IP
    if (config.getMqttIP() != nullptr && ((hasValidMQTTIP() && *config.getMqttIP() != readMQTTIP()) || !hasValidMQTTIP())) {
      write_successful &= writeMQTTIP(*config.getMqttIP());
    } else if (config.getMqttIP() == nullptr && hasValidMQTTIP()) {
      write_successful &= writeMQTTIP(IPAddress(0, 0, 0, 0));
    }

    // Write MQTT Port
    if (config.getMqttPort() != nullptr && ((hasValidMQTTPort() && *config.getMqttPort() != readMQTTPort()) || !hasValidMQTTPort())) {
      write_successful &= writeMQTTPort(*config.getMqttPort());
    } else if (config.getMqttPort() == nullptr && hasValidMQTTPort()) {
      write_successful &= writeMQTTPort(0);
    }

    // Write MQTT Username
    if (config.getMqttUsername() != nullptr && ((hasValidMQTTUsername() && *config.getMqttUsername() != readMQTTUsername()) || !hasValidMQTTUsername())) {
      write_successful &= writeMQTTUsername(*config.getMqttUsername());
    } else if (config.getMqttUsername() == nullptr && hasValidMQTTUsername()) {
      write_successful &= writeMQTTUsername(0);
    }

    // Write MQTT PW
    if (config.getMqttPassword() != nullptr && ((hasValidMQTTPassword() && *config.getMqttPassword() != readMQTTPassword()) || !hasValidMQTTPassword())) {
      write_successful &= writeMQTTPassword(*config.getMqttPassword());
    } else if (config.getMqttPassword() == nullptr && hasValidMQTTPassword()) {
      write_successful &= writeMQTTPassword(0);
    }

    resetGadgets();

    for (auto gadget_data: config.getGadgets()) {
      uint8_t type = std::get<0>(gadget_data);
      bitfield_set bitfield = std::get<1>(gadget_data);
      pin_set ports = std::get<2>(gadget_data);
      std::string name = std::get<3>(gadget_data);
      std::string gadget_config = std::get<4>(gadget_data);
      std::string code_config = std::get<5>(gadget_data);

      auto status = writeGadget(type, bitfield, ports, name, gadget_config, code_config);
      if (status != WriteGadgetStatus::WritingOK) {
        write_successful = false;
      }
    }

    return write_successful;
  }
};
