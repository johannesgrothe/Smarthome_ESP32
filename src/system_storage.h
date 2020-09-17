#pragma once

#include "ArduinoJson.h"
#include <EEPROM.h>
#include <sstream>
#include <utility>
#include <cmath>
#include "system_settings.h"
#include "console_logger.h"

// config flag
#define CONFIG_CHECK_BYTE 0
#define CONFIG_CHECK_INDEX_ID 0
#define CONFIG_CHECK_INDEX_WIFI_SSID 1
#define CONFIG_CHECK_INDEX_WIFI_PW 2
#define CONFIG_CHECK_INDEX_MQTT_IP 3
#define CONFIG_CHECK_INDEX_MQTT_PORT 4
#define CONFIG_CHECK_INDEX_MQTT_USER 5
#define CONFIG_CHECK_INDEX_MQTT_PW 6

// id
#define ID_POS 5
#define ID_MAX_LEN 35

//wifi_ssid
#define WIFI_SSID_POS (ID_POS + ID_MAX_LEN + 1)
#define WIFI_SSID_MAX_LEN 50

//wifi_pw
#define WIFI_PW_POS (WIFI_SSID_POS + WIFI_SSID_MAX_LEN + 1)
#define WIFI_PW_MAX_LEN 50

//wifi_pw
#define MQTT_IP_POS (WIFI_PW_POS + WIFI_PW_MAX_LEN + 1)
#define MQTT_IP_MAX_LEN 15

//wifi_port
#define MQTT_PORT_POS (MQTT_IP_POS + MQTT_IP_MAX_LEN + 1)
#define MQTT_PORT_MAX_LEN 6

//wifi_port
#define MQTT_USER_POS (MQTT_PORT_POS + MQTT_PORT_MAX_LEN + 1)
#define MQTT_USER_MAX_LEN 50

//wifi_port
#define MQTT_PW_POS (MQTT_USER_POS + MQTT_USER_MAX_LEN)
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
    uint8_t content_flag = EEPROM.readByte(CONFIG_CHECK_BYTE);
    content_flag = calculateNewContentFlag(index, value, content_flag);
    EEPROM.writeByte(CONFIG_CHECK_BYTE, content_flag);
    EEPROM.commit();
  }

  static bool getContentFlag(uint8_t index) {
    uint8_t content_flag = EEPROM.readByte(CONFIG_CHECK_BYTE);
    auto mask = (unsigned int) pow(2, index);
    uint8_t content_info = content_flag & mask;
    return content_info != 0;
  }

  // TODO: remove
  static bool writeConfigStr(const char *config_str) {
    logger.println(LOG_TYPE::INFO, "Writing...");
    bool inside_string = false;
    int write_index = 0;
    int chars_deleted = 0;
    int chars_written = 0;
    logger.incIndent();
    logger.print(LOG_TYPE::DATA, "");

    char write_buffer[EEPROM_CONFIG_LEN_MAX]{};
    for (int i = 0; i < strlen(config_str); i++) {
      char write_char = char(config_str[i]);

      if (write_char == '"')
        inside_string = !inside_string;

      if (write_char == '\n' || (write_char == ' ' && !inside_string)) {
        chars_deleted++;
      } else {
        chars_written++;
        logger.add(write_char);
        EEPROM.writeChar(write_index, config_str[i]);
        write_buffer[write_index] = config_str[i];
        write_index++;
      }
    }
    EEPROM.commit();

    logger.addln();
    logger.print(LOG_TYPE::DATA, "Bytes written: ");
    logger.addln(chars_written);
    logger.print(LOG_TYPE::DATA, "Bytes deleted: ");
    logger.addln(chars_deleted);
    logger.decIndent();
    bool everything_ok = true;
    for (int i = 0; i < write_index; i++) {
      everything_ok = everything_ok && EEPROM.readChar(i) == write_buffer[i];
    }
    logger.println(LOG_TYPE::INFO, "Validating written Bytes...");
    logger.incIndent();
    if (everything_ok)
      logger.println(LOG_TYPE::INFO, "Successfull");
    else
      logger.println(LOG_TYPE::ERR, "Validation failed.");
    return everything_ok;
  }

  static bool writeContent(int start, int max_len, std::string content) {
    int id_length = content.size();
    if (id_length > max_len) {
      logger.println(LOG_TYPE::ERR, "written content is too long");
      return false;
      id_length = max_len;
    }
    for (int pos = 0; pos < id_length; pos++) {
      EEPROM.writeChar(pos + start, content[pos]);
    }
    EEPROM.writeChar(id_length + start, '\0');
    EEPROM.commit();
    return true;
  }

  static std::string readContent(int start, int max_len) {
    std::stringstream ss;
    for (int pos = 0; pos < max_len; pos++) {
      char c = EEPROM.readChar(pos + start);
      if (c == '\n') {
        return ss.str();
      }
      ss << c;
    }
    return ss.str();
  }

public:

  static void resetContentFlag() {
    uint8_t content_flag = 0;
    EEPROM.writeByte(CONFIG_CHECK_BYTE, content_flag);
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

  // read + write ID
  /**
   * Writes the chip identifier to the eeprom
   * @param id the chip id to be written
   * @return whether writing was successful
   */
  static bool writeID(std::string id) {
    auto success = writeContent(ID_POS, ID_MAX_LEN, std::move(id));
    setContentFlag(CONFIG_CHECK_INDEX_ID, success);
    return success;
  }

  /**
   * Reads the chip identifier from the eeprom
   * @return the chip identifier
   */
  static std::string readID() {
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
  static bool writeWifiSSID(std::string ssid) {
    auto success = writeContent(WIFI_SSID_POS, WIFI_SSID_MAX_LEN, std::move(ssid));
    setContentFlag(CONFIG_CHECK_INDEX_WIFI_SSID, success);
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
  static bool writeWifiPW(std::string pw) {
    auto success = writeContent(WIFI_PW_POS, WIFI_PW_MAX_LEN, std::move(pw));
    setContentFlag(CONFIG_CHECK_INDEX_WIFI_PW, success);
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
  static bool writeMQTTIP(std::string ip) {
    auto success = writeContent(MQTT_IP_POS, MQTT_IP_MAX_LEN, std::move(ip));
    setContentFlag(CONFIG_CHECK_INDEX_MQTT_IP, success);
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
   * @param pw the port to be written
   * @return whether writing was successful
   */
  static bool writeMQTTPort(std::string port) {
    auto success = writeContent(MQTT_PORT_POS, MQTT_PORT_MAX_LEN, std::move(port));
    setContentFlag(CONFIG_CHECK_INDEX_MQTT_PORT, success);
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
