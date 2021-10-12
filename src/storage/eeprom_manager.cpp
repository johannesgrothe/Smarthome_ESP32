#include "eeprom_writer.h"

uint8_t EepromWriter::calculateNewContentFlag(uint8_t index, bool new_value, uint8_t bitfield) {
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

bool EepromWriter::getValueFromContentFlag(uint8_t index, uint8_t bitfield) {
  auto mask = (unsigned int) pow(2, index);
  uint8_t content_info = bitfield & mask;
  return content_info != 0;
}

void EepromWriter::setFlag(int bitfield_address, uint8_t index, bool value) {
  uint8_t content_flag = EEPROM.readByte(bitfield_address);
  content_flag = calculateNewContentFlag(index, value, content_flag);
  EEPROM.writeByte(bitfield_address, content_flag);
  EEPROM.commit();
}

bool EepromWriter::getFlag(int bitfield_address, uint8_t index) {
  uint8_t content_flag = EEPROM.readByte(bitfield_address);
  return getValueFromContentFlag(index, content_flag);
}

void EepromWriter::setContentFlag(uint8_t index, bool value) {
  setFlag(VALID_CONFIG_BITFIELD_BYTE, index, value);
}

bool EepromWriter::getContentFlag(uint8_t index) {
  return getFlag(VALID_CONFIG_BITFIELD_BYTE, index);
}

bool EepromWriter::writeUInt8(int pos, uint8_t value) {
  EEPROM.writeByte(pos, value);
  EEPROM.commit();
  return true;
}

uint8_t EepromWriter::readUInt8(int pos) {
  return EEPROM.readByte(pos);
}

bool EepromWriter::writeUInt16(int pos, uint16_t value) {

  uint8_t first = value >> (uint8_t) 8;
  uint8_t second = value & (uint16_t) 0x00ff;

  bool success = writeUInt8(pos, first);
  success = success & writeUInt8(pos + 1, second);

  return success;
}

uint16_t EepromWriter::readUInt16(int pos) {
  auto first = readUInt8(pos);
  auto second = readUInt8(pos + 1);
  return (uint16_t) ((first * (0xFF + 1)) + second);
}

bool EepromWriter::writeString(int start, int max_len, std::string content) {
  int id_length = content.size();
  if (id_length > max_len) {
    logger_e("EepromWriter", "Written content is too long");
    return false;
  }
  for (int pos = 0; pos < id_length; pos++) {
    EEPROM.writeChar(pos + start, content[pos]);
  }
  EEPROM.writeChar(id_length + start, '\0');
  EEPROM.commit();
  return true;
}

std::string EepromWriter::readString(int start, int max_len) {
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

uint8_t EepromWriter::getGadgetCount() {
  return readUInt8(GADGET_COUNT_POS);
}

bool EepromWriter::writeGadgetCount(uint8_t count) {
  return writeUInt8(GADGET_COUNT_POS, count);
}

uint16_t EepromWriter::getGadgetMemoryStart(uint8_t gadget_nr) {
  if (gadget_nr >= GADGET_MAX_COUNT) {
    return 0;
  }
  if (gadget_nr == 0) {
    return GADGET_BLOCK_START;
  }
  return readUInt16(GADGET_POS_START + (gadget_nr * 2));
}

uint16_t EepromWriter::getGadgetMemoryEnd(uint8_t gadget_nr) {
  if (gadget_nr >= GADGET_MAX_COUNT) {
    return 0;
  }
  return readUInt16(GADGET_POS_START + ((gadget_nr + 1) * 2));
}

bool EepromWriter::setGadgetMemoryEnd(uint8_t gadget_nr, uint16_t mem_end) {
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

WriteGadgetStatus
EepromWriter::writeNewGadget(uint8_t gadget_type, bitfield_set config_bf, pin_set ports, const std::string &name,
                              const std::string &gadget_json, const std::string &code_json) {

  // Check if updating gadget is possible
  uint8_t gadget_index = getGadgetCount();

  // Check if maximum gadget count is reached
  if (gadget_index >= GADGET_MAX_COUNT) {
    logger_e("EepromWriter", "Cannot save gadget: maximum count of gadgets reached");
    return WriteGadgetStatus::MaxGadgetCountReached;
  }

  // Check if name exist and quit if name is already taken
  auto existing_names = readAllGadgetNames();
  for (const auto &list_name: existing_names) {
    if (name == list_name) {
      logger_e("EepromWriter", "Cannot save gadget: gadget name '%s' is already in use", name.c_str());
      return WriteGadgetStatus::NameAlreadyInUse;
    }
  }

  auto existing_ports = readAllGadgetPorts();

  for (auto gadget_port: ports) {
    // Check if port is configured on the system
    auto buf_pin = getPinForPort(gadget_port);
    if (!buf_pin && gadget_port) {
      logger_e("EepromWriter", "Cannot save gadget: port '%d' is not configured on this system", gadget_port);
      return WriteGadgetStatus::PortNotConfigured;
    }

    // Check if port is already in use on the system
    for (auto existing_port: existing_ports) {
      if (gadget_port == existing_port) {
        logger_e("EepromWriter", "Cannot save gadget: gadget tries to use port already occupied (%d)", gadget_port);
        return WriteGadgetStatus::PortAlreadyInUse;
      }
    }
  }

  // Address the gadget start at
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
    logger_e("EepromWriter", "Cannot save gadget: missing space in eeprom");
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
    logger_e("EepromWriter", "Cannot save gadget: error writing content");
    return WriteGadgetStatus::ErrorWritingContent;
  }

  // set the starting point for the next gadget
  if (!setGadgetMemoryEnd(gadget_index, end_index)) {
    logger_e("EepromWriter", "Cannot save gadget: error saving gadget memory end");
    return WriteGadgetStatus::ErrorSavingMemoryEnd;
  }

  // increment gadget count to formally "save" the gadget
  writeGadgetCount(gadget_index + 1);
  return WriteGadgetStatus::WritingOK;
}

int EepromWriter::getGadgetIndexForName(const std::string &name) {
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

gadget_tuple EepromWriter::readGadget(uint8_t gadget_index) {
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

std::vector<uint8_t> EepromWriter::readAllGadgetPorts() {
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

std::vector<std::string> EepromWriter::readAllGadgetNames() {
  auto gadgets = readAllGadgets();
  std::vector<std::string> names;
  for (auto gadget: gadgets) {
    auto buf_name = std::get<3>(gadget);
    names.push_back(buf_name);
  }
  return names;
}

std::vector<gadget_tuple> EepromWriter::readAllGadgets() {
  auto gadget_count = getGadgetCount();
  std::vector<gadget_tuple> gadgets;
  for (uint8_t i = 0; i < gadget_count; i++) {
    gadget_tuple buf_g = readGadget(i);
    gadgets.push_back(buf_g);
  }
  return gadgets;
}

WriteGadgetStatus
EepromWriter::writeGadget(uint8_t gadget_type, bitfield_set config_bf, pin_set ports, const std::string &name,
                           const std::string &gadget_json, const std::string &code_json) {

  if (gadget_type >= GadgetIdentifierCount) {
    logger_e("EepromWriter", "Unknown gadget identifier '%d'", gadget_type);
    return WriteGadgetStatus::GadgetTypeError0;
  }

  auto type = (GadgetIdentifier) gadget_type;

  if (type == GadgetIdentifier::err_type) {
    logger_e("EepromWriter", "Cannot save gadget: gadget has err-type 0");
    return WriteGadgetStatus::GadgetTypeErrorUnknown;
  } else {
    logger_i("EepromWriter", "Saving gadget '%s' with type '%d'", name.c_str(), gadget_type);
  }

  DynamicJsonDocument buf_doc(2000);

  // Check gadget config
  if (!gadget_json.empty()) {
    auto err = deserializeJson(buf_doc, gadget_json);
    if (err != DeserializationError::Ok) {
      logger_e("EepromWriter", "Cannot save gadget: received faulty gadget config");
      return WriteGadgetStatus::FaultyConfigJSON;
    }
  }

  // Check code config
  if (!code_json.empty()) {
    auto err = deserializeJson(buf_doc, code_json);
    if (err != DeserializationError::Ok) {
      logger_e("EepromWriter", "Cannot save gadget: received faulty code config");
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

bool EepromWriter::deleteGadget(uint8_t gadget_index) {
  auto gadget_count = getGadgetCount();

  if (gadget_count == 0) {
    logger_e("EepromWriter", "Cannot delete gadget: no gadget saved");
    return false;
  }

  if (gadget_index > gadget_count - 1) {
    logger_e("EepromWriter", "Cannot delete gadget: index does not exist");
    return false;
  }

  if (gadget_index == gadget_count - 1) {
    return writeGadgetCount(gadget_count - 1);
  }

  std::vector<gadget_tuple> gadgets;

  for (uint8_t i = gadget_index + 1; i < gadget_count; i++) {
    auto buf_gadget = readGadget(i);
    gadgets.push_back(buf_gadget);
  }

  if (!writeGadgetCount(gadget_index)) {
    logger_e("EepromWriter", "Error in deleting process while updating gadget count");
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
      logger_e("EepromWriter", "Error in in deletion process: moving gadgets failed");
      return false;
    }
  }
  return true;
}

bool EepromWriter::resetGadgets() {
  return writeGadgetCount(0);
}

bool EepromWriter::writeIRrecvPin(uint8_t pin) {
  return writeUInt8(IR_RECV_PIN_POS, pin);
}

uint8_t EepromWriter::readIRrecvPin() {
  return readUInt8(IR_RECV_PIN_POS);
}

bool EepromWriter::writeIrSendPin(uint8_t pin) {
  return writeUInt8(IR_SEND_PIN_POS, pin);
}

uint8_t EepromWriter::readIrSendPin() {
  return readUInt8(IR_SEND_PIN_POS);
}

bool EepromWriter::writeRadioRecvPin(uint8_t pin) {
  return writeUInt8(RADIO_RECV_POS, pin);
}

uint8_t EepromWriter::readRadioRecvPin() {
  return readUInt8(RADIO_RECV_POS);
}

bool EepromWriter::writeRadioSendPin(uint8_t pin) {
  return writeUInt8(RADIO_SEND_POS, pin);
}

uint8_t EepromWriter::readRadioSendPin() {
  return readUInt8(RADIO_SEND_POS);
}

bool EepromWriter::writeNetworkMode(NetworkMode mode) {
  return writeUInt8(NETWORK_MODE_POS, (uint8_t) mode);
}

NetworkMode EepromWriter::readNetworkMode() {
  uint8_t mode = readUInt8(NETWORK_MODE_POS);
  if (mode < NetworkModeCount) {
    return (NetworkMode) mode;
  }
  return NetworkMode::None;
}

bool EepromWriter::writeID(const std::string &id) {
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

std::string EepromWriter::readID() {
  auto id_str = readString(ID_POS, ID_MAX_LEN);

  if (!hasValidID() || !validate_utf8(id_str)) {
    std::stringstream s_str;
    s_str << "esp_" << random_int(10000);
    writeID(s_str.str());
  }
  return id_str;
}

bool EepromWriter::hasValidID() {
  return getContentFlag(CONFIG_CHECK_INDEX_ID);
}

bool EepromWriter::writeWifiSSID(const std::string &ssid) {
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

std::string EepromWriter::readWifiSSID() {
  return readString(WIFI_SSID_POS, WIFI_SSID_MAX_LEN);
}

bool EepromWriter::hasValidWifiSSID() {
  return getContentFlag(CONFIG_CHECK_INDEX_WIFI_SSID);
}

bool EepromWriter::writeWifiPW(const std::string &pw) {
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

std::string EepromWriter::readWifiPW() {
  return readString(WIFI_PW_POS, WIFI_PW_MAX_LEN);
}

bool EepromWriter::hasValidWifiPW() {
  return getContentFlag(CONFIG_CHECK_INDEX_WIFI_PW);
}

bool EepromWriter::writeMQTTIP(const IPContainer &ip) {
  bool success = true;
  auto data = ip.getData();
  for (int i = 0; i < 4; i++) {
    success = success && writeUInt8(MQTT_IP_POS + i, data[i]);
  }
  if (ip == IPContainer(0, 0, 0, 0)) {
    setContentFlag(CONFIG_CHECK_INDEX_MQTT_IP, false);
    success = true;
  } else {
    setContentFlag(CONFIG_CHECK_INDEX_MQTT_IP, success);
  }
  return success;
}

IPContainer EepromWriter::readMQTTIP() {
  uint8_t data[4];
  for (int i = 0; i < 4; i++) {
    data[i] = readUInt8(MQTT_IP_POS + i);
  }
  return {data[0], data[1], data[2], data[3]};
}

bool EepromWriter::hasValidMQTTIP() {
  return getContentFlag(CONFIG_CHECK_INDEX_MQTT_IP);
}

bool EepromWriter::writeMQTTPort(uint16_t port) {
  bool success = writeUInt16(MQTT_PORT_POS, port);

  if (port == 0) {
    setContentFlag(CONFIG_CHECK_INDEX_MQTT_PORT, false);
    success = true;
  } else {
    setContentFlag(CONFIG_CHECK_INDEX_MQTT_PORT, success);
  }
  return success;
}

uint16_t EepromWriter::readMQTTPort() {
  return readUInt16(MQTT_PORT_POS);
}

bool EepromWriter::hasValidMQTTPort() {
  return getContentFlag(CONFIG_CHECK_INDEX_MQTT_PORT);
}

bool EepromWriter::writeMQTTUsername(const std::string &username) {
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

std::string EepromWriter::readMQTTUsername() {
  return readString(MQTT_USER_POS, MQTT_USER_MAX_LEN);
}

bool EepromWriter::hasValidMQTTUsername() {
  return getContentFlag(CONFIG_CHECK_INDEX_MQTT_USER);
}

bool EepromWriter::writeMQTTPassword(const std::string &pw) {
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

std::string EepromWriter::readMQTTPassword() {
  return readString(MQTT_PW_POS, MQTT_PW_MAX_LEN);
}

bool EepromWriter::hasValidMQTTPassword() {
  return getContentFlag(CONFIG_CHECK_INDEX_MQTT_PW);
}

std::string EepromWriter::readWholeEEPROM() {
  std::stringstream ss;
  for (int i = 0; i < 500; i++) {
    char buf_char = EEPROM.readChar(i);
    if (buf_char == 0) {
      ss << "\\n";
    } else {
      ss << buf_char;
    }
  }
  return ss.str();
}

void EepromWriter::eraseEeprom() {
  for (int i = 0; i < EEPROM_SIZE; i++) {
    if (i % 2) {
      EEPROM.writeChar(i, '-');
    } else {
      EEPROM.writeChar(i, '_');
    }
  }

  writeUInt8(GADGET_COUNT_POS, 0);
  writeUInt8(VALID_CONFIG_BITFIELD_BYTE, 0);
  writeUInt8(SYSTEM_SETTINGS_BITFIELD_BYTE, 0);

  EEPROM.commit();
}

void EepromWriter::resetContentFlag() {
  uint8_t content_flag = 0;
  EEPROM.writeByte(VALID_CONFIG_BITFIELD_BYTE, content_flag);
  EEPROM.commit();
}

bool EepromWriter::initEEPROM() {
  logger_i("EepromStorage", "Initializing EEPROM...");

  if (!EEPROM.begin(EEPROM_SIZE)) {
    logger_e("EepromStorage", "Initializing EEPROM failed.");
    return false;
  }
  return true;
}
