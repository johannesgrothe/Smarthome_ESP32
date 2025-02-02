#pragma once

#include <sstream>
#include <string>

#ifndef UNIT_TEST
#include <EEPROM.h>
#else
#include "../test_dummys/dummy_eeprom.h"
#endif

#include <system/variants.h>

#include "../ip_container.h"

// region CONSTANTS

// valid client config bitfield
#define VALID_CLIENT_CONFIG_BITFIELD_BYTE 0

#define CONFIG_CHECK_INDEX_ID 0
#define CONFIG_CHECK_INDEX_WIFI_SSID 1
#define CONFIG_CHECK_INDEX_WIFI_PW 2
#define CONFIG_CHECK_INDEX_MQTT_IP 3
#define CONFIG_CHECK_INDEX_MQTT_PORT 4
#define CONFIG_CHECK_INDEX_MQTT_USER 5
#define CONFIG_CHECK_INDEX_MQTT_PW 6

// valid eol config bitfield
#define VALID_EOL_CONFIG_BITFIELD_BYTE 1

#define CONFIG_CHECK_HW_VAR 0
#define CONFIG_CHECK_HW_SERIAL 1

// config checksums
#define CLIENT_CFG_CRC 2
#define EOL_CFG_CRC 4

#define DATA_START 10

// hw variant
#define HW_VARIANT_POS (DATA_START)

// hw serial
#define HW_SERIAL_POS (HW_VARIANT_POS + 1)
#define HW_SERIAL_MAX_LEN 16

// id
#define ID_POS (HW_SERIAL_POS + HW_SERIAL_MAX_LEN + 1)
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

// endregion

class EepromManager {
public:
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
    static void setSystemConfigFlag(uint8_t index, bool value);

    /**
     * Returns the bit in the content bitfield at the selected index
     * @param index the index the bit is located at
     * @return the value of the bit at the selected index
     */
    static bool getSystemConfigFlag(uint8_t index);

    /**
     * Sets the bit at the eol bitfield to the selected value
     * @param index the index the byte is located at
     * @param value the value to set it to
     */
    static void setEolConfigFlag(uint8_t index, bool value);

    /**
     * Returns the bit in the eol bitfield at the selected index
     * @param index the index the bit is located at
     * @return the value of the bit at the selected index
     */
    static bool getEolConfigFlag(uint8_t index);

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
    static bool writeString(int start, int max_len, const std::string &content);

    /**
     * Reads the sontent of the eeprom as a string
     * @param start start-index of the string
     * @param max_len maximum length of the string to read
     * @return the read content
     */
    static std::string readString(int start, int max_len);

    /**
     * Writes the Hardware Variant to the eeprom.
     * @param variant the Hardware Variant to be written
     * @return whether writing was successful
     */
    static bool writeHwVariant(HwVariant variant);

    /**
     * Reads the Hardware Variant from the eeprom
     * @return the Hardware Variant
     */
    static HwVariant readHwVariant();

    /**
     * Checks if the there is a valid Hardware Variant stored in the EEPROM
     * @return whether there is a valid Hardware Variant
     */
    static bool hasValidHwVariant();

    // read + write HW serial
    /**
     * Writes the hardware serial to the eeprom
     * @param serial the serial to be written
     * @return whether writing was successful
     */
    static bool writeHwSerial(const std::string &serial);

    /**
     * Reads the hardware serial from the eeprom
     * @return the hardware serial
     */
    static std::string readHwSerial();

    /**
     * Checks if the there is a valid serial stored in the EEPROM
     * @return whether there is a valid serial
     */
    static bool hasValidHwSerial();

    // read + write ID
    /**
     * Writes the chip identifier to the eeprom
     * @param id the chip id to be written
     * @return whether writing was successful
     */
    static bool writeID(const std::string &id);

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
    static bool writeWifiSSID(const std::string &ssid);

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
    static bool writeWifiPW(const std::string &pw);

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
};
