#include "eeprom_storage.h"

#include "../string_methods.h"
#include "eeprom_manager.h"

EepromStorage::EepromStorage() {
  initialized_ = EepromManager::initEEPROM();

  if (initialized_) {
    logger_i("EepromStorage", "EEPROM usage: %d / %d bytes", getEEPROMUsage(), EEPROM_SIZE);
  }
}

bool EepromStorage::eraseAllConfigs() {
  EepromManager::eraseEeprom();
  return true;
}

bool EepromStorage::saveSystemConfig(SystemConfig config) {

  // Write ID
  bool write_successful = EepromManager::writeID(config.id);

  // Write Network Mode
  write_successful &= EepromManager::writeNetworkMode(config.network_mode);

  // Write Pins
  write_successful &= EepromManager::writeIRrecvPin(config.ir_recv_pin);
  write_successful &= EepromManager::writeIrSendPin(config.ir_send_pin);
  write_successful &= EepromManager::writeRadioRecvPin(config.radio_recv_pin);
  write_successful &= EepromManager::writeRadioSendPin(config.radio_send_pin);

  // Write Wifi SSID
  if (config.wifi_ssid != nullptr &&
      ((EepromManager::hasValidWifiSSID() && *config.wifi_ssid != EepromManager::readWifiSSID()) || !EepromManager::hasValidWifiSSID())) {
    write_successful &= EepromManager::writeWifiSSID(*config.wifi_ssid);
  } else if (config.wifi_ssid == nullptr && EepromManager::hasValidWifiSSID()) {
    write_successful &= EepromManager::writeWifiSSID("");
  }

  // Write Wifi PW
  if (config.wifi_pw != nullptr &&
      ((EepromManager::hasValidWifiPW() && *config.wifi_pw != EepromManager::readWifiPW()) || !EepromManager::hasValidWifiPW())) {
    write_successful &= EepromManager::writeWifiPW(*config.wifi_pw);
  } else if (config.wifi_pw == nullptr && EepromManager::hasValidWifiPW()) {
    write_successful &= EepromManager::writeWifiPW("");
  }

  // Write MQTT IP
  if (config.mqtt_ip != nullptr &&
      ((EepromManager::hasValidMQTTIP() && *config.mqtt_ip != EepromManager::readMQTTIP()) || !EepromManager::hasValidMQTTIP())) {
    write_successful &= EepromManager::writeMQTTIP(*config.mqtt_ip);
  } else if (config.mqtt_ip == nullptr && EepromManager::hasValidMQTTIP()) {
    write_successful &= EepromManager::writeMQTTIP(IPContainer(0, 0, 0, 0));
  }

  // Write MQTT Port
  if (config.mqtt_port != nullptr &&
      ((EepromManager::hasValidMQTTPort() && *config.mqtt_port != EepromManager::readMQTTPort()) || !EepromManager::hasValidMQTTPort())) {
    write_successful &= EepromManager::writeMQTTPort(*config.mqtt_port);
  } else if (config.mqtt_port == nullptr && EepromManager::hasValidMQTTPort()) {
    write_successful &= EepromManager::writeMQTTPort(0);
  }

  // Write MQTT Username
  if (config.mqtt_username != nullptr &&
      ((EepromManager::hasValidMQTTUsername() && *config.mqtt_username != EepromManager::readMQTTUsername()) || !EepromManager::hasValidMQTTUsername())) {
    write_successful &= EepromManager::writeMQTTUsername(*config.mqtt_username);
  } else if (config.mqtt_username == nullptr && EepromManager::hasValidMQTTUsername()) {
    write_successful &= EepromManager::writeMQTTUsername("");
  }

  // Write MQTT PW
  if (config.mqtt_password != nullptr &&
      ((EepromManager::hasValidMQTTPassword() && *config.mqtt_password != EepromManager::readMQTTPassword()) || !EepromManager::hasValidMQTTPassword())) {
    write_successful &= EepromManager::writeMQTTPassword(*config.mqtt_password);
  } else if (config.mqtt_password == nullptr && EepromManager::hasValidMQTTPassword()) {
    write_successful &= EepromManager::writeMQTTPassword("");
  }

  if (write_successful) {
    write_successful &= EepromManager::writeUInt16(CLIENT_CFG_CRC, config.crc16());
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
  std::string id = EepromManager::readID();
  NetworkMode network_mode = EepromManager::readNetworkMode();
  auto gadgets = EepromManager::readAllGadgets();

  uint8_t ir_recv = EepromManager::readIRrecvPin();
  uint8_t ir_send = EepromManager::readIrSendPin();

  uint8_t radio_recv = EepromManager::readRadioRecvPin();
  uint8_t radio_send = EepromManager::readRadioSendPin();

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

  auto stored_crc16 = EepromManager::readUInt16(CLIENT_CFG_CRC);

  if (config->crc16() != stored_crc16) {
    logger_e("EEPROMStorage", "Config checksums do not match (%d / %d)", stored_crc16, config->crc16());
    return nullptr;
  }

  return config;
}

std::shared_ptr<GadgetConfig> EepromStorage::loadGadgetConfig() {
  return nullptr; //TODO: implement
}

bool EepromStorage::saveGadgetConfig(GadgetConfig config) {

  EepromManager::resetGadgets();

  bool write_successful = true;

  for (auto gadget_data: config.gadgets) {
    uint8_t type = std::get<0>(gadget_data);
    bitfield_set bitfield = std::get<1>(gadget_data);
    port_set ports = std::get<2>(gadget_data);
    std::string name = std::get<3>(gadget_data);
    std::string gadget_config = std::get<4>(gadget_data);
    std::string code_config = std::get<5>(gadget_data);

    auto status = EepromManager::writeGadget(type, bitfield, ports, name, gadget_config, code_config);
    if (status != WriteGadgetStatus::WritingOK) {
      write_successful = false;
    }
  }

  return write_successful;
}

std::shared_ptr<EventConfig> EepromStorage::loadEventConfig() {
  auto event_data = EepromManager::readEventMapping();
  auto out_cfg = std::make_shared<EventConfig>(event_data);
  return out_cfg;
}

bool EepromStorage::saveEventConfig(EventConfig config) {
  auto status = EepromManager::writeEventMapping(config.event_mapping);

  if (!status) {
    return false;
  }

  auto test_cfg = loadEventConfig();
  if (test_cfg == nullptr) {
    return false;
  }

  if (config != *test_cfg) {
    return false;
  }
  return true;
}


uint16_t EepromStorage::getEEPROMUsage() {
  return EepromManager::getGadgetMemoryEnd(EepromManager::getGadgetCount() - 1);
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
