#include "eeprom_manager.h"

#include <random.h>
#include <string_methods.h>

#include "eeprom_gadget_mapping_coder.h"

uint8_t EepromManager::calculateNewContentFlag(const uint8_t index, const bool new_value, const uint8_t bitfield) {
    auto content_flag = bitfield;
    auto mask = static_cast<unsigned int>(pow(2, index));
    if (new_value) {
        content_flag = content_flag | mask;
    } else {
        mask = 0xFF - mask;
        content_flag = content_flag & mask;
    }
    return content_flag;
}

bool EepromManager::getValueFromContentFlag(const uint8_t index, const uint8_t bitfield) {
    const auto mask = static_cast<unsigned int>(pow(2, index));
    const uint8_t content_info = bitfield & mask;
    return content_info != 0;
}

void EepromManager::setFlag(const int bitfield_address, const uint8_t index, const bool value) {
    uint8_t content_flag = EEPROM.readByte(bitfield_address);
    content_flag = calculateNewContentFlag(index, value, content_flag);
    EEPROM.writeByte(bitfield_address, content_flag);
    EEPROM.commit();
}

bool EepromManager::getFlag(const int bitfield_address, const uint8_t index) {
    const uint8_t content_flag = EEPROM.readByte(bitfield_address);
    return getValueFromContentFlag(index, content_flag);
}

void EepromManager::setSystemConfigFlag(const uint8_t index, const bool value) {
    setFlag(VALID_CLIENT_CONFIG_BITFIELD_BYTE, index, value);
}

bool EepromManager::getSystemConfigFlag(const uint8_t index) {
    return getFlag(VALID_CLIENT_CONFIG_BITFIELD_BYTE, index);
}

void EepromManager::setEolConfigFlag(const uint8_t index, const bool value) {
    setFlag(VALID_EOL_CONFIG_BITFIELD_BYTE, index, value);
}

bool EepromManager::getEolConfigFlag(const uint8_t index) {
    return getFlag(VALID_EOL_CONFIG_BITFIELD_BYTE, index);
}

bool EepromManager::writeUInt8(const int pos, const uint8_t value) {
    EEPROM.writeByte(pos, value);
    EEPROM.commit();
    return true;
}

uint8_t EepromManager::readUInt8(const int pos) {
    return EEPROM.readByte(pos);
}

bool EepromManager::writeUInt16(const int pos, const uint16_t value) {
    const uint8_t first = value >> static_cast<uint8_t>(8);
    const uint8_t second = value & static_cast<uint16_t>(0x00ff);

    bool success = writeUInt8(pos, first);
    success = success & writeUInt8(pos + 1, second);

    return success;
}

uint16_t EepromManager::readUInt16(const int pos) {
    const auto first = readUInt8(pos);
    const auto second = readUInt8(pos + 1);
    return static_cast<uint16_t>((first * (0xFF + 1)) + second);
}

bool EepromManager::writeString(const int start, const int max_len, const std::string &content) {
    const unsigned int id_length = content.size();
    if (id_length > max_len) {
        logger_e("EepromManager", "Written content is too long");
        return false;
    }
    for (int pos = 0; pos < id_length; pos++) {
        EEPROM.writeChar(pos + start, content[pos]);
    }
    EEPROM.writeChar(id_length + start, '\0');
    EEPROM.commit();
    return true;
}

std::string EepromManager::readString(const int start, const int max_len) {
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

bool EepromManager::writeHwVariant(HwVariant variant) {
    const auto value = static_cast<uint8_t>(variant);
    const bool success = writeUInt8(HW_VARIANT_POS, value);
    setEolConfigFlag(CONFIG_CHECK_HW_VAR, success);
    return success;
}

HwVariant EepromManager::readHwVariant() {
    if (!hasValidHwVariant()) {
        return HwVariant::unknown;
    }
    auto value = readUInt8(HW_VARIANT_POS);
    return static_cast<HwVariant>(value);
}

bool EepromManager::hasValidHwVariant() {
    return getEolConfigFlag(CONFIG_CHECK_HW_VAR);
}

bool EepromManager::writeHwSerial(const std::string &serial) {
    bool success;
    if (serial.empty()) {
        setSystemConfigFlag(CONFIG_CHECK_HW_SERIAL, false);
        success = true;
    } else {
        success = writeString(HW_SERIAL_POS, HW_SERIAL_MAX_LEN, serial);
        setSystemConfigFlag(CONFIG_CHECK_HW_SERIAL, success);
    }
    return success;
}

std::string EepromManager::readHwSerial() {
    if (!hasValidHwSerial()) {
        return "not_set";
    }
    return readString(WIFI_PW_POS, WIFI_PW_MAX_LEN);
}

bool EepromManager::hasValidHwSerial() {
    return getEolConfigFlag(CONFIG_CHECK_HW_SERIAL);
}


bool EepromManager::writeID(const std::string &id) {
    bool success;
    if (id.empty()) {
        setSystemConfigFlag(CONFIG_CHECK_INDEX_ID, false);
        success = true;
    } else {
        success = writeString(ID_POS, ID_MAX_LEN, id);
        setSystemConfigFlag(CONFIG_CHECK_INDEX_ID, success);
    }
    return success;
}

std::string EepromManager::readID() {
    auto id_str = readString(ID_POS, ID_MAX_LEN);

    if (!hasValidID() || !validate_utf8(id_str)) {
        std::stringstream s_str;
        s_str << "esp_" << random_int(10000);
        writeID(s_str.str());
    }
    return id_str;
}

bool EepromManager::hasValidID() {
    return getSystemConfigFlag(CONFIG_CHECK_INDEX_ID);
}

bool EepromManager::writeWifiSSID(const std::string &ssid) {
    bool success;
    if (ssid.empty()) {
        setSystemConfigFlag(CONFIG_CHECK_INDEX_WIFI_SSID, false);
        success = true;
    } else {
        success = writeString(WIFI_SSID_POS, WIFI_SSID_MAX_LEN, ssid);
        setSystemConfigFlag(CONFIG_CHECK_INDEX_WIFI_SSID, success);
    }
    return success;
}

std::string EepromManager::readWifiSSID() {
    return readString(WIFI_SSID_POS, WIFI_SSID_MAX_LEN);
}

bool EepromManager::hasValidWifiSSID() {
    return getSystemConfigFlag(CONFIG_CHECK_INDEX_WIFI_SSID);
}

bool EepromManager::writeWifiPW(const std::string &pw) {
    bool success;
    if (pw.empty()) {
        setSystemConfigFlag(CONFIG_CHECK_INDEX_WIFI_PW, false);
        success = true;
    } else {
        success = writeString(WIFI_PW_POS, WIFI_PW_MAX_LEN, pw);
        setSystemConfigFlag(CONFIG_CHECK_INDEX_WIFI_PW, success);
    }
    return success;
}

std::string EepromManager::readWifiPW() {
    return readString(WIFI_PW_POS, WIFI_PW_MAX_LEN);
}

bool EepromManager::hasValidWifiPW() {
    return getSystemConfigFlag(CONFIG_CHECK_INDEX_WIFI_PW);
}

bool EepromManager::writeMQTTIP(const IPContainer &ip) {
    bool success = true;
    for (int i = 0; i < 4; i++) {
        success = success && writeUInt8(MQTT_IP_POS + i, ip[i]);
    }
    if (ip == IPContainer(0, 0, 0, 0)) {
        setSystemConfigFlag(CONFIG_CHECK_INDEX_MQTT_IP, false);
        success = true;
    } else {
        setSystemConfigFlag(CONFIG_CHECK_INDEX_MQTT_IP, success);
    }
    return success;
}

IPContainer EepromManager::readMQTTIP() {
    uint8_t data[4];
    for (int i = 0; i < 4; i++) {
        data[i] = readUInt8(MQTT_IP_POS + i);
    }
    return {data[0], data[1], data[2], data[3]};
}

bool EepromManager::hasValidMQTTIP() {
    return getSystemConfigFlag(CONFIG_CHECK_INDEX_MQTT_IP);
}

bool EepromManager::writeMQTTPort(const uint16_t port) {
    bool success = writeUInt16(MQTT_PORT_POS, port);

    if (port == 0) {
        setSystemConfigFlag(CONFIG_CHECK_INDEX_MQTT_PORT, false);
        success = true;
    } else {
        setSystemConfigFlag(CONFIG_CHECK_INDEX_MQTT_PORT, success);
    }
    return success;
}

uint16_t EepromManager::readMQTTPort() {
    return readUInt16(MQTT_PORT_POS);
}

bool EepromManager::hasValidMQTTPort() {
    return getSystemConfigFlag(CONFIG_CHECK_INDEX_MQTT_PORT);
}

bool EepromManager::writeMQTTUsername(const std::string &username) {
    bool success = writeString(MQTT_USER_POS, MQTT_USER_MAX_LEN, username);
    if (username.empty()) {
        setSystemConfigFlag(CONFIG_CHECK_INDEX_MQTT_USER, false);
        success = true;
    } else {
        setSystemConfigFlag(CONFIG_CHECK_INDEX_MQTT_USER, success);
    }
    return success;
}

std::string EepromManager::readMQTTUsername() {
    return readString(MQTT_USER_POS, MQTT_USER_MAX_LEN);
}

bool EepromManager::hasValidMQTTUsername() {
    return getSystemConfigFlag(CONFIG_CHECK_INDEX_MQTT_USER);
}

bool EepromManager::writeMQTTPassword(const std::string &pw) {
    bool success = writeString(MQTT_PW_POS, MQTT_PW_MAX_LEN, pw);
    if (pw.empty()) {
        setSystemConfigFlag(CONFIG_CHECK_INDEX_MQTT_PW, false);
        success = true;
    } else {
        setSystemConfigFlag(CONFIG_CHECK_INDEX_MQTT_PW, success);
    }
    return success;
}

std::string EepromManager::readMQTTPassword() {
    return readString(MQTT_PW_POS, MQTT_PW_MAX_LEN);
}

bool EepromManager::hasValidMQTTPassword() {
    return getSystemConfigFlag(CONFIG_CHECK_INDEX_MQTT_PW);
}

std::string EepromManager::readWholeEEPROM() {
    std::stringstream ss;
    for (int i = 0; i < 500; i++) {
        const char buf_char = EEPROM.readChar(i);
        if (buf_char == 0) {
            ss << "\\n";
        } else {
            ss << buf_char;
        }
    }
    return ss.str();
}

void EepromManager::eraseEeprom() {
    for (int i = 0; i < EEPROM_SIZE; i++) {
        if (i % 2) {
            EEPROM.writeChar(i, '-');
        } else {
            EEPROM.writeChar(i, '_');
        }
    }

    writeUInt8(VALID_CLIENT_CONFIG_BITFIELD_BYTE, 0);
    writeUInt8(VALID_EOL_CONFIG_BITFIELD_BYTE, 0);

    EEPROM.commit();
}

void EepromManager::resetContentFlag() {
    constexpr uint8_t content_flag = 0;
    EEPROM.writeByte(VALID_CLIENT_CONFIG_BITFIELD_BYTE, content_flag);
    EEPROM.commit();
}

bool EepromManager::initEEPROM() {
    logger_i("EepromStorage", "Initializing EEPROM...");

    if (!EEPROM.begin(EEPROM_SIZE)) {
        logger_e("EepromStorage", "Initializing EEPROM failed.");
        return false;
    }
    return true;
}
