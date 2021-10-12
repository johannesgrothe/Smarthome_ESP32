#include "eeprom_storage.h"

#include "../string_methods.h"
#include "../random.h"

EepromStorage::EepromStorage() {
  initialized_ = initEEPROM();

  if (initialized_) {
    logger_i("EepromStorage", "EEPROM usage: %d / %d bytes", getEEPROMUsage(), EEPROM_SIZE);
  }
}

bool EepromStorage::eraseAllConfigs() {
  eraseEeprom();
  return true;
}

bool EepromStorage::saveSystemConfig(SystemConfig config) {

  // Write ID
  bool write_successful = writeID(config.id);

  // Write Network Mode
  write_successful &= writeNetworkMode(config.network_mode);

  // Write Pins
  write_successful &= writeIRrecvPin(config.ir_recv_pin);
  write_successful &= writeIrSendPin(config.ir_send_pin);
  write_successful &= writeRadioRecvPin(config.radio_recv_pin);
  write_successful &= writeRadioSendPin(config.radio_send_pin);

  // Write Wifi SSID
  if (config.wifi_ssid != nullptr &&
      ((hasValidWifiSSID() && *config.wifi_ssid != readWifiSSID()) || !hasValidWifiSSID())) {
    write_successful &= writeWifiSSID(*config.wifi_ssid);
  } else if (config.wifi_ssid == nullptr && hasValidWifiSSID()) {
    write_successful &= writeWifiSSID("");
  }

  // Write Wifi PW
  if (config.wifi_pw != nullptr &&
      ((hasValidWifiPW() && *config.wifi_pw != readWifiPW()) || !hasValidWifiPW())) {
    write_successful &= writeWifiPW(*config.wifi_pw);
  } else if (config.wifi_pw == nullptr && hasValidWifiPW()) {
    write_successful &= writeWifiPW("");
  }

  // Write MQTT IP
  if (config.mqtt_ip != nullptr &&
      ((hasValidMQTTIP() && *config.mqtt_ip != readMQTTIP()) || !hasValidMQTTIP())) {
    write_successful &= writeMQTTIP(*config.mqtt_ip);
  } else if (config.mqtt_ip == nullptr && hasValidMQTTIP()) {
    write_successful &= writeMQTTIP(IPContainer(0, 0, 0, 0));
  }

  // Write MQTT Port
  if (config.mqtt_port != nullptr &&
      ((hasValidMQTTPort() && *config.mqtt_port != readMQTTPort()) || !hasValidMQTTPort())) {
    write_successful &= writeMQTTPort(*config.mqtt_port);
  } else if (config.mqtt_port == nullptr && hasValidMQTTPort()) {
    write_successful &= writeMQTTPort(0);
  }

  // Write MQTT Username
  if (config.mqtt_username != nullptr &&
      ((hasValidMQTTUsername() && *config.mqtt_username != readMQTTUsername()) || !hasValidMQTTUsername())) {
    write_successful &= writeMQTTUsername(*config.mqtt_username);
  } else if (config.mqtt_username == nullptr && hasValidMQTTUsername()) {
    write_successful &= writeMQTTUsername("");
  }

  // Write MQTT PW
  if (config.mqtt_password != nullptr &&
      ((hasValidMQTTPassword() && *config.mqtt_password != readMQTTPassword()) || !hasValidMQTTPassword())) {
    write_successful &= writeMQTTPassword(*config.mqtt_password);
  } else if (config.mqtt_password == nullptr && hasValidMQTTPassword()) {
    write_successful &= writeMQTTPassword("");
  }

  if (write_successful) {
    write_successful &= writeUInt16(CLIENT_CFG_CRC, config.crc16());
  }

  auto loaded_config = loadSystemConfig();
  if (loaded_config == nullptr) {
    return false;
  }
  if (config != *loaded_config) {
    return false;
  }

  return write_successful;
}

std::shared_ptr<SystemConfig> EepromStorage::loadSystemConfig() {
  std::string id = readID();
  NetworkMode network_mode = readNetworkMode();
  auto gadgets = readAllGadgets();

  uint8_t ir_recv = readIRrecvPin();
  uint8_t ir_send = readIrSendPin();

  uint8_t radio_recv = readRadioRecvPin();
  uint8_t radio_send = readRadioSendPin();

  std::shared_ptr<std::string> wifi_ssid = nullptr;
  std::shared_ptr<std::string> wifi_pw = nullptr;

  std::shared_ptr<IPContainer> mqtt_ip = nullptr;
  std::shared_ptr<uint16_t> mqtt_port = nullptr;

  std::shared_ptr<std::string> mqtt_username = nullptr;
  std::shared_ptr<std::string> mqtt_pw = nullptr;

  if (hasValidWifiSSID()) {
    wifi_ssid = std::make_shared<std::string>(readWifiSSID());
  }

  if (hasValidWifiPW()) {
    wifi_pw = std::make_shared<std::string>(readWifiPW());
  }

  if (hasValidMQTTIP()) {
    mqtt_ip = std::make_shared<IPContainer>(readMQTTIP());
  }

  if (hasValidMQTTPort()) {
    mqtt_port = std::make_shared<uint16_t>(readMQTTPort());
  }

  if (hasValidMQTTUsername()) {
    mqtt_username = std::make_shared<std::string>(readMQTTUsername());
  }

  if (hasValidMQTTPassword()) {
    mqtt_pw = std::make_shared<std::string>(readMQTTPassword());
  }

  auto config = std::make_shared<SystemConfig>(id,
                                               network_mode,
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

  auto stored_crc16 = readUInt16(CLIENT_CFG_CRC);

  if (config->crc16() != stored_crc16) {
    logger_e("EEPROMStorage", "Config checksums do not match (%d / %d)", stored_crc16, config->crc16());
    return nullptr;
  }

  return config;
}

std::shared_ptr<GadgetConfig> EepromStorage::loadGadgetConfig() {
  return nullptr;
}

bool EepromStorage::saveGadgetConfig(GadgetConfig config) {

  resetGadgets();

  bool write_successful = true;

  for (auto gadget_data: config.gadgets) {
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

std::shared_ptr<EventConfig> EepromStorage::loadEventConfig() {
  return nullptr;
}

bool EepromStorage::saveEventConfig(EventConfig config) {
  return false;
}


uint16_t EepromStorage::getEEPROMUsage() {
  return getGadgetMemoryEnd(getGadgetCount() - 1);
}

std::string EepromStorage::getEepromLayout() {
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
  ss << "\nclient config checksum: " << CLIENT_CFG_CRC << " - " << (CLIENT_CFG_CRC + 1);
  ss << "\nevent mapping checksum: " << EVENT_MAP_CRC << " - " << (EVENT_MAP_CRC + 1);
  ss << "\ngadget config checksum: " << GADGET_CFG_CRC << " - " << (GADGET_CFG_CRC + 1);
  ss << "\nid: " << ID_POS << " - " << ID_POS + ID_MAX_LEN;
  ss << "\nwifi_ssid: " << WIFI_SSID_POS << " - " << WIFI_SSID_POS + WIFI_SSID_MAX_LEN;
  ss << "\nwifi_pw: " << WIFI_PW_POS << " - " << WIFI_PW_POS + WIFI_PW_MAX_LEN;
  ss << "\nmqtt_ip: " << MQTT_IP_POS << " - " << MQTT_IP_POS + MQTT_IP_MAX_LEN;
  ss << "\nmqtt_port: " << MQTT_PORT_POS << " - " << MQTT_PORT_POS + MQTT_PORT_MAX_LEN;
  ss << "\nmqtt_user: " << MQTT_USER_POS << " - " << MQTT_USER_POS + MQTT_USER_MAX_LEN;
  ss << "\nmqtt_pw: " << MQTT_PW_POS << " - " << MQTT_PW_POS + MQTT_PW_MAX_LEN;
  return ss.str();
}
