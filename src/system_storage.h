#pragma once

#include <ArduinoJson.h>
#include <EEPROM.h>
#include <sstream>
#include <utility>
#include <cmath>
#include "system_settings.h"
#include "console_logger.h"

#define EEPROM_WRITE_ALWAYS

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
#define VALID_GADGET_BITFIELD_BYTE 1

// valid remote storage bitfield
#define SYSTEM_SETTINGS_BITFIELD_BYTE 2

// ir
#define IR_RECV_PIN_POS 3
#define IR_SEND_PIN_POS 4

// radio
#define RADIO_PIN_POS 4

// network mode
#define NETWORK_MODE_POS 6

// remotes
#define GADGET_REMOTE_POS 7
#define CODE_REMOTE_POS GADGET_REMOTE_POS + 1
#define EVENT_REMOTE_POS CODE_REMOTE_POS + 1

// id
#define ID_POS 10
#define ID_MAX_LEN 20

//wifi_ssid
#define WIFI_SSID_POS (ID_POS + ID_MAX_LEN + 1)
#define WIFI_SSID_MAX_LEN 25

//wifi_pw
#define WIFI_PW_POS (WIFI_SSID_POS + WIFI_SSID_MAX_LEN + 1)
#define WIFI_PW_MAX_LEN 50

//wifi_pw
#define MQTT_IP_POS (WIFI_PW_POS + WIFI_PW_MAX_LEN + 1)
#define MQTT_IP_MAX_LEN 15

//mqtt port
#define MQTT_PORT_POS (MQTT_IP_POS + MQTT_IP_MAX_LEN + 1)
#define MQTT_PORT_MAX_LEN 6

//mqtt username
#define MQTT_USER_POS (MQTT_PORT_POS + MQTT_PORT_MAX_LEN + 1)
#define MQTT_USER_MAX_LEN 50

//mqtt password
#define MQTT_PW_POS (MQTT_USER_POS + MQTT_USER_MAX_LEN + 1)
#define MQTT_PW_MAX_LEN 50

static bool validateJson(const char *new_json_str) {
  DynamicJsonDocument json_file(2048);
  DeserializationError err = deserializeJson(json_file, new_json_str);
  return err == DeserializationError::Ok;
}

class System_Storage {
private:

  static uint8_t calculateNewContentFlag(uint8_t index, bool new_value, uint8_t old_flag) {
    auto content_flag = old_flag;
    auto mask = (unsigned int) pow(2, index);
    if (new_value) {
      content_flag = content_flag | mask;
    } else {
      mask = 0xFF - mask;
      content_flag = content_flag & mask;
    }
    return content_flag;
  }

  static void setContentFlag(uint8_t index, bool value) {
    uint8_t content_flag = EEPROM.readByte(VALID_CONFIG_BITFIELD_BYTE);
    content_flag = calculateNewContentFlag(index, value, content_flag);
    EEPROM.writeByte(VALID_CONFIG_BITFIELD_BYTE, content_flag);
    EEPROM.commit();
  }

  static bool getContentFlag(uint8_t index) {
    uint8_t content_flag = EEPROM.readByte(VALID_CONFIG_BITFIELD_BYTE);
    auto mask = (unsigned int) pow(2, index);
    uint8_t content_info = content_flag & mask;
    return content_info != 0;
  }

  static bool writeContent(int start, int max_len, std::string content) {
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

  static bool writeByte(int pos, uint8_t value) {
    EEPROM.writeByte(pos, value);
    EEPROM.commit();
    return true;
  }

  static uint8_t readByte(int pos) {
    return EEPROM.readByte(pos);
  }

  static std::string readContent(int start, int max_len) {
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

public:

  static void printEEPROMLayout() {
    std::stringstream ss;
    ss << "EEPROM config:";
    ss << "\nvalid config bitfield: " << VALID_CONFIG_BITFIELD_BYTE;
    ss << "\nvalid gadget bitfield: " << VALID_GADGET_BITFIELD_BYTE;
    ss << "\nsystem settings bitfield: " << SYSTEM_SETTINGS_BITFIELD_BYTE;
    ss << "\nir_recv pin: " << IR_RECV_PIN_POS;
    ss << "\nir_send pin: " << IR_SEND_PIN_POS;
    ss << "\nradio pin: " << RADIO_PIN_POS;
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

  static void resetContentFlag() {
    uint8_t content_flag = 0;
    EEPROM.writeByte(VALID_CONFIG_BITFIELD_BYTE, content_flag);
    EEPROM.commit();
  }

  // init eeprom
  /**
   * Initializes the system EEPROM
   * @return whether the EEPROM was correctly initialized
   */
  static bool initEEPROM() {
    logger.println("Initializing EEPROM...");

    if (!EEPROM.begin(EEPROM_CONFIG_LEN_MAX)) {
      logger.println(LOG_TYPE::ERR, "failed to initialize EEPROM");
      return false;
    }
    return true;
  }

  // read and write gadgets
  /**
   * Writes the gadget config in any free eeprom storage
   * @param config gadget configuration information
   * @return whether writing the info was successful
   */
  static bool writeGadget(const DynamicJsonDocument& config) {
    return false;
  }

  // read + write IR pins
  /**
   * Writes the ir receive pin to the eeprom
   * @param pin the pin used for ir receiver
   * @return whether writing was successful
   */
  static bool writeIRrecvPin(uint8_t pin) {
    return writeByte(IR_RECV_PIN_POS, pin);
  }

  /**
   * Reads the ir receive pin from the eeprom
   * @return the ir recv pin
   */
  static uint8_t readIRrecvPin() {
    return readByte(IR_RECV_PIN_POS);
  }

  /**
   * Writes the ir send pin to the eeprom
   * @param pin the pin used for ir sender
   * @return whether writing was successful
   */
  static bool writeIRsendPin(uint8_t pin) {
    return writeByte(IR_SEND_PIN_POS, pin);
  }

  /**
   * Reads the ir send pin from the eeprom
   * @return the ir send pin
   */
  static uint8_t readIRsendPin() {
    return readByte(IR_SEND_PIN_POS);
  }

  // read + write radio pin
  /**
 * Writes the radio pin to the eeprom
 * @param pin the pin used for radio
 * @return whether writing was successful
 */
  static bool writeRadioPin(uint8_t pin) {
    return writeByte(RADIO_PIN_POS, pin);
  }

  /**
   * Reads the radio pin from the eeprom
   * @return the radio pin
   */
  static uint8_t readRadioPin() {
    return readByte(RADIO_PIN_POS);
  }

  // read + write network mode
  /**
   * Writes the network mode to the eeprom
   * @param mode the mode the network should use
   * @return whether writing was successful
   */
  static bool writeNetworkMode(uint8_t mode) {
    return writeByte(NETWORK_MODE_POS, mode);
  }

  /**
   * Reads the network mode from the eeprom
   * @return the network mode
   */
  static uint8_t readNetworkMode() {
    return readByte(NETWORK_MODE_POS);
  }

  // read + write gadget remote
  /**
   * Writes the gadget remote mode to the eeprom.
   * Writing a 0 is interpreted as 'no remote used'.
   * @param mode the mode used for the gadget remote
   * @return whether writing was successful
   */
  static bool writeGadgetRemote(uint8_t mode) {
    return writeByte(GADGET_REMOTE_POS, mode);
  }

  /**
   * Reads the gadget remote mode from the eeprom
   * @return the gadget remote mode
   */
  static uint8_t readGadgetRemote() {
    return readByte(GADGET_REMOTE_POS);
  }

  // read + write code remote
  /**
   * Writes the code remote mode to the eeprom.
   * Writing a 0 is interpreted as 'no remote used'.
   * @param mode the mode used for the code remote
   * @return whether writing was successful
   */
  static bool writeCodeRemote(uint8_t mode) {
    return writeByte(CODE_REMOTE_POS, mode);
  }

  /**
   * Reads the code remote mode from the eeprom
   * @return the code remote mode
   */
  static uint8_t readCodeRemote() {
    return readByte(CODE_REMOTE_POS);
  }

  // read + write event remote
  /**
   * Writes the event remote mode to the eeprom.
   * Writing a 0 is interpreted as 'no remote used'.
   * @param mode the mode used for the event remote
   * @return whether writing was successful
   */
  static bool writeEventRemote(uint8_t mode) {
    return writeByte(EVENT_REMOTE_POS, mode);
  }

  /**
   * Reads the event remote mode from the eeprom
   * @return the gadget remote mode
   */
  static uint8_t readEventRemote() {
    return readByte(EVENT_REMOTE_POS);
  }

  // read + write ID
  /**
   * Writes the chip identifier to the eeprom
   * @param id the chip id to be written
   * @return whether writing was successful
   */
  static bool writeID(const std::string& id) {
    bool success;
    if (id == "null") {
      setContentFlag(CONFIG_CHECK_INDEX_ID, false);
      success = true;
    } else {
      success = writeContent(ID_POS, ID_MAX_LEN, id);
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
    return readContent(ID_POS, ID_MAX_LEN);
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
    if (ssid == "null") {
      setContentFlag(CONFIG_CHECK_INDEX_WIFI_SSID, false);
      success = true;
    } else {
      success = writeContent(WIFI_SSID_POS, WIFI_SSID_MAX_LEN, ssid);
      setContentFlag(CONFIG_CHECK_INDEX_WIFI_SSID, success);
    }
    return success;
  }

  /**
   * Reads the WIFI SSID from the eeprom
   * @return the wifi ssid
   */
  static std::string readWifiSSID() {
    return readContent(WIFI_SSID_POS, WIFI_SSID_MAX_LEN);
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
    if (pw == "null") {
      setContentFlag(CONFIG_CHECK_INDEX_WIFI_PW, false);
      success = true;
    } else {
      success = writeContent(WIFI_PW_POS, WIFI_PW_MAX_LEN, pw);
      setContentFlag(CONFIG_CHECK_INDEX_WIFI_PW, success);
    }
    return success;
  }

  /**
   * Reads the WIFI password from the eeprom
   * @return the wifi password
   */
  static std::string readWifiPW() {
    return readContent(WIFI_PW_POS, WIFI_PW_MAX_LEN);
  }

  /**
   * Checks if the there is a valid WIFI password stored in the EEPROM
   * @return whether there is a valid password
   */
  static bool hasValidWifiPW() {
    return getContentFlag(CONFIG_CHECK_INDEX_WIFI_PW);
  }

  /**
 * Writes the MQTT IP-Address to the eeprom
 * @param ip the ip to be written
 * @return whether writing was successful
 */
  static bool writeMQTTIP(const std::string& ip) {
    bool success;
    if (ip == "null") {
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_PW, false);
      success = true;
    } else {
      success = writeContent(MQTT_PW_POS, MQTT_PW_MAX_LEN, ip);
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_PW, success);
    }
    return success;
  }

  /**
   * Reads the MQTT IP-Address from the eeprom
   * @return the port
   */
  static std::string readMQTTIP() {
    return readContent(MQTT_IP_POS, MQTT_IP_MAX_LEN);
  }

  /**
   * Checks if the there is a valid MQTT IP-Address stored in the EEPROM
   * @return whether there is a valid ip
   */
  static bool hasValidMQTTIP() {
    return getContentFlag(CONFIG_CHECK_INDEX_MQTT_IP);
  }

  /**
   * Writes the MQTT port to the eeprom
   * @param port the port to be written
   * @return whether writing was successful
   */
  static bool writeMQTTPort(const std::string& port) {
    bool success;
#ifdef EEPROM_WRITE_ALWAYS
    success = writeContent(MQTT_PORT_POS, MQTT_PORT_MAX_LEN, port);
#endif
    if (port == "null") {
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_PORT, false);
      success = true;
    } else {
#ifndef EEPROM_WRITE_ALWAYS
      success = writeContent(MQTT_PORT_POS, MQTT_PORT_MAX_LEN, port);
#endif
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_PORT, success);
    }
    return success;
  }

  /**
   * Reads the MQTT port from the eeprom
   * @return the port
   */
  static std::string readMQTTPort() {
    return readContent(MQTT_PORT_POS, MQTT_PORT_MAX_LEN);
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
#ifdef EEPROM_WRITE_ALWAYS
    success = writeContent(MQTT_USER_POS, MQTT_USER_MAX_LEN, username);
#endif
    if (username == "null") {
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_USER, false);
      success = true;
    } else {
#ifndef EEPROM_WRITE_ALWAYS
      success = writeContent(MQTT_USER_POS, MQTT_USER_MAX_LEN, username);
#endif
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_USER, success);
    }
    return success;
  }

  /**
   * Reads the MQTT username from the eeprom
   * @return the username
   */
  static std::string readMQTTUsername() {
    return readContent(MQTT_USER_POS, MQTT_USER_MAX_LEN);
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
#ifdef EEPROM_WRITE_ALWAYS
    success = writeContent(MQTT_PW_POS, MQTT_PW_MAX_LEN, pw);
#endif
    if (pw == "null") {
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_PW, false);
      success = true;
    } else {
#ifndef EEPROM_WRITE_ALWAYS
      success = writeContent(MQTT_PW_POS, MQTT_PW_MAX_LEN, pw);
#endif
      setContentFlag(CONFIG_CHECK_INDEX_MQTT_PW, success);
    }
    return success;
  }

  /**
   * Reads the MQTT password from the eeprom
   * @return the password
   */
  static std::string readMQTTPassword() {
    return readContent(MQTT_PW_POS, MQTT_PW_MAX_LEN);
  }

  /**
   * Checks if the there is a valid MQTT password stored in the EEPROM
   * @return whether there is a valid password
   */
  static bool hasValidMQTTPassword() {
    return getContentFlag(CONFIG_CHECK_INDEX_MQTT_PW);
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
   * Fills EEPROM with repeating '0123456789'-sequences
   */
  static void writeTestEEPROM() {
    for (int i = 0; i < 500; i++) {
      char k = (char) (48 + (i % 10));
      EEPROM.writeChar(i, k);
    }
    EEPROM.commit();
  }

};
// TODO: split into declaration and implementation
