#include "eeprom_storage.h"

#include <console_logger.h>

#include "eeprom_manager.h"

static auto TAG = "EepromStorage";

EepromStorage::EepromStorage() {
    initialized_ = EepromManager::initEEPROM();

    if (initialized_) {
        logger_i(TAG, "EEPROM usage: %d / %d bytes", getEEPROMUsage(), EEPROM_SIZE);
    }
}

bool EepromStorage::eraseAllConfigs() {
    EepromManager::eraseEeprom();
    return true;
}

bool EepromStorage::saveSystemConfig(const SystemConfig config) {
    logger_i(TAG, "Writing system config");
    // Write ID
    bool write_successful = EepromManager::writeID(config.id);

    // Write Wifi SSID
    if (config.wifi_ssid != nullptr &&
        ((EepromManager::hasValidWifiSSID() && *config.wifi_ssid != EepromManager::readWifiSSID()) ||
         !EepromManager::hasValidWifiSSID())) {
        write_successful &= EepromManager::writeWifiSSID(*config.wifi_ssid);
    } else if (config.wifi_ssid == nullptr && EepromManager::hasValidWifiSSID()) {
        write_successful &= EepromManager::writeWifiSSID("");
    }

    // Write Wifi PW
    if (config.wifi_pw != nullptr &&
        ((EepromManager::hasValidWifiPW() && *config.wifi_pw != EepromManager::readWifiPW()) ||
         !EepromManager::hasValidWifiPW())) {
        write_successful &= EepromManager::writeWifiPW(*config.wifi_pw);
    } else if (config.wifi_pw == nullptr && EepromManager::hasValidWifiPW()) {
        write_successful &= EepromManager::writeWifiPW("");
    }

    // Write MQTT IP
    if (config.mqtt_ip != nullptr &&
        ((EepromManager::hasValidMQTTIP() && *config.mqtt_ip != EepromManager::readMQTTIP()) ||
         !EepromManager::hasValidMQTTIP())) {
        write_successful &= EepromManager::writeMQTTIP(*config.mqtt_ip);
    } else if (config.mqtt_ip == nullptr && EepromManager::hasValidMQTTIP()) {
        write_successful &= EepromManager::writeMQTTIP(IPContainer(0, 0, 0, 0));
    }

    // Write MQTT Port
    if (config.mqtt_port != nullptr &&
        ((EepromManager::hasValidMQTTPort() && *config.mqtt_port != EepromManager::readMQTTPort()) ||
         !EepromManager::hasValidMQTTPort())) {
        write_successful &= EepromManager::writeMQTTPort(*config.mqtt_port);
    } else if (config.mqtt_port == nullptr && EepromManager::hasValidMQTTPort()) {
        write_successful &= EepromManager::writeMQTTPort(0);
    }

    // Write MQTT Username
    if (config.mqtt_username != nullptr &&
        ((EepromManager::hasValidMQTTUsername() && *config.mqtt_username != EepromManager::readMQTTUsername()) ||
         !EepromManager::hasValidMQTTUsername())) {
        write_successful &= EepromManager::writeMQTTUsername(*config.mqtt_username);
    } else if (config.mqtt_username == nullptr && EepromManager::hasValidMQTTUsername()) {
        write_successful &= EepromManager::writeMQTTUsername("");
    }

    // Write MQTT PW
    if (config.mqtt_password != nullptr &&
        ((EepromManager::hasValidMQTTPassword() && *config.mqtt_password != EepromManager::readMQTTPassword()) ||
         !EepromManager::hasValidMQTTPassword())) {
        write_successful &= EepromManager::writeMQTTPassword(*config.mqtt_password);
    } else if (config.mqtt_password == nullptr && EepromManager::hasValidMQTTPassword()) {
        write_successful &= EepromManager::writeMQTTPassword("");
    }

    if (write_successful) {
        write_successful &= EepromManager::writeUInt16(CLIENT_CFG_CRC, config.crc16());
    }

    const auto loaded_config = loadSystemConfig();
    if (loaded_config == nullptr) {
        return false;
    }
    if (config != *loaded_config) {
        return false;
    }

    return write_successful;
}

std::shared_ptr<SystemConfig> EepromStorage::loadSystemConfig() {
    std::string id = EepromManager::readID();

    std::shared_ptr<std::string> wifi_ssid = nullptr;
    std::shared_ptr<std::string> wifi_pw = nullptr;

    std::shared_ptr<IPContainer> mqtt_ip = nullptr;
    std::shared_ptr<uint16_t> mqtt_port = nullptr;

    std::shared_ptr<std::string> mqtt_username = nullptr;
    std::shared_ptr<std::string> mqtt_pw = nullptr;

    if (EepromManager::hasValidWifiSSID()) {
        wifi_ssid = std::make_shared<std::string>(EepromManager::readWifiSSID());
    }

    if (EepromManager::hasValidWifiPW()) {
        wifi_pw = std::make_shared<std::string>(EepromManager::readWifiPW());
    }

    if (EepromManager::hasValidMQTTIP()) {
        mqtt_ip = std::make_shared<IPContainer>(EepromManager::readMQTTIP());
    }

    if (EepromManager::hasValidMQTTPort()) {
        mqtt_port = std::make_shared<uint16_t>(EepromManager::readMQTTPort());
    }

    if (EepromManager::hasValidMQTTUsername()) {
        mqtt_username = std::make_shared<std::string>(EepromManager::readMQTTUsername());
    }

    if (EepromManager::hasValidMQTTPassword()) {
        mqtt_pw = std::make_shared<std::string>(EepromManager::readMQTTPassword());
    }

    auto config = std::make_shared<SystemConfig>(id,
                                                 wifi_ssid,
                                                 wifi_pw,
                                                 mqtt_ip,
                                                 mqtt_port,
                                                 mqtt_username,
                                                 mqtt_pw);

    const auto stored_crc16 = EepromManager::readUInt16(CLIENT_CFG_CRC);

    if (config->crc16() != stored_crc16) {
        logger_e("EEPROMStorage", "Config checksums do not match (%d / %d)", stored_crc16, config->crc16());
        return nullptr;
    }

    return config;
}


bool EepromStorage::saveEolConfig(EolConfig config) {
    logger_i(TAG, "Writing eol config");
    // Write ID
    bool write_successful = EepromManager::writeHwVariant(config.variant);

    // Write HW Serial
    write_successful &= EepromManager::writeHwSerial(config.serial);

    if (write_successful) {
        write_successful &= EepromManager::writeUInt16(EOL_CFG_CRC, config.crc16());
    }

    const auto loaded_config = loadEolConfig();
    if (loaded_config == nullptr) {
        return false;
    }
    if (config != *loaded_config) {
        return false;
    }

    return write_successful;
}

std::shared_ptr<EolConfig> EepromStorage::loadEolConfig() {
    HwVariant variant = EepromManager::readHwVariant();
    std::string serial = EepromManager::readHwSerial();

    auto config = std::make_shared<EolConfig>(variant, serial);

    // TODO: this should be a method, and all read/write commands should probably be protected
    const auto stored_crc16 = EepromManager::readUInt16(EOL_CFG_CRC);

    if (config->crc16() != stored_crc16) {
        logger_e("EEPROMStorage", "Config checksums do not match (%d / %d)", stored_crc16, config->crc16());
        return nullptr;
    }

    return config;
}


std::string EepromStorage::getEepromLayout() {
    // TODO: incomplete
    std::stringstream ss;
    ss << "EEPROM config:";
    ss << "\nvalid config bitfield: " << VALID_CLIENT_CONFIG_BITFIELD_BYTE;
    ss << "\nclient config checksum: " << CLIENT_CFG_CRC << " - " << (CLIENT_CFG_CRC + 1);
    ss << "\neol config checksum: " << EOL_CFG_CRC << " - " << (EOL_CFG_CRC + 1);
    ss << "\nid: " << ID_POS << " - " << ID_POS + ID_MAX_LEN;
    ss << "\nwifi_ssid: " << WIFI_SSID_POS << " - " << WIFI_SSID_POS + WIFI_SSID_MAX_LEN;
    ss << "\nwifi_pw: " << WIFI_PW_POS << " - " << WIFI_PW_POS + WIFI_PW_MAX_LEN;
    ss << "\nmqtt_ip: " << MQTT_IP_POS << " - " << MQTT_IP_POS + MQTT_IP_MAX_LEN;
    ss << "\nmqtt_port: " << MQTT_PORT_POS << " - " << MQTT_PORT_POS + MQTT_PORT_MAX_LEN;
    ss << "\nmqtt_user: " << MQTT_USER_POS << " - " << MQTT_USER_POS + MQTT_USER_MAX_LEN;
    ss << "\nmqtt_pw: " << MQTT_PW_POS << " - " << MQTT_PW_POS + MQTT_PW_MAX_LEN;
    return ss.str();
}
