#pragma once

#include "../json_check.h"

#ifdef STATIC_CONFIG_ACTIVE
#include "static_storage.h"
#else

#include "eeprom_storage.h"

#endif

#include "config.h"

/**
 * System storage class handling EEPROM saving
 */
class System_Storage {
private:

/**
 * Writes a gadget to the eeprom
 * @param gadget_type type of the gadget
 * @param remote_bf bitfield for the remotes
 * @param ports ports used by the gadget to connect hardware
 * @param gadget_config base config for the gadget
 * @param code_config config for the code mapping
 * @return whether writing was successful
 */
  static WriteGadgetStatus
  writeGadget(uint8_t gadget_type, bitfield_set remote_bf, pin_set ports, const std::string &name,
              const std::string &gadget_config, const std::string &code_config) {
    return EEPROM_Storage::writeGadget(gadget_type, remote_bf, ports, name, gadget_config, code_config);
  }

/**
 * Writes a gadget from the config json body
 * @param json_body JSON-data to save the gadget from
 * @return (Whether writing was successful | Status-Message)
 */
  WriteGadgetStatus writeGadget(DynamicJsonDocument json_body) {
    auto type = json_body["type"].as<uint8_t>();

    auto name = json_body["name"].as<std::string>();

    uint8_t port0 = 0;
    uint8_t port1 = 0;
    uint8_t port2 = 0;
    uint8_t port3 = 0;
    uint8_t port4 = 0;

    if (json_body.containsKey("ports")) {
      JsonObject ports = json_body["ports"].as<JsonObject>();
      if (ports.containsKey("port0")) {
        port0 = ports["port0"].as<uint8_t>();
      }
      if (ports.containsKey("port1")) {
        port1 = ports["port1"].as<uint8_t>();
      }
      if (ports.containsKey("port2")) {
        port2 = ports["port2"].as<uint8_t>();
      }
      if (ports.containsKey("port3")) {
        port3 = ports["port3"].as<uint8_t>();
      }
      if (ports.containsKey("port4")) {
        port4 = ports["port4"].as<uint8_t>();
      }
    }

    pin_set pins = {port0, port1, port2, port3, port4};

    std::string gadget_config;
    std::string code_config;

    if (json_body.containsKey("config")) {
      gadget_config = json_body["config"].as<std::string>();
    }
    if (json_body.containsKey("codes")) {
      code_config = json_body["codes"].as<std::string>();
    }

    // Create bitfield
    bitfield_set remote_bf = {false, false, false, false, false, false, false, false};

    if (json_body.containsKey("remotes")) {
      JsonObject remote_json = json_body["remotes"].as<JsonObject>();
      if (remote_json.containsKey("gadget")) {
        remote_bf[0] = remote_json["gadget"].as<bool>();
      }
      if (remote_json.containsKey("code")) {
        remote_bf[1] = remote_json["code"].as<bool>();
      }
      if (remote_json.containsKey("event")) {
        remote_bf[2] = remote_json["event"].as<bool>();
      }
    }

    auto status = writeGadget(type, remote_bf, pins, name, gadget_config, code_config);
    return status;
  }

/**
 * Writes the value of a param to the eeprom
 * @param param_name Name of the param to write
 * @param param_val Value of the param as string
 * @param param_val_uint Value of the param as uint
 * @return whether writing was successful
 */
  bool writeConfigParam(const std::string &param_name, const std::string &param_val, uint8_t param_val_uint) {
    logger.printfln("Write param '%s'", param_name.c_str());
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
      if (param_val == "null") {
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

    return write_successful;
  }

  /**
   * Writes and applies complete config file
   * @param config Config to write
   * @return Whether writing was successful
   */
  bool writeConfig(DynamicJsonDocument config) {
    logger.println("Writing config");
    logger.incIndent();

    logger.println("Writing system preferences");
    logger.incIndent();

    bool writing_data_successful = true;

    // Write system preferences
    if (config.containsKey("data")) {
      const JsonObject preference_data = config["data"];
      for (auto param_name: config_keys) {
        if (preference_data.containsKey(param_name)) {

          // Extract Data
          std::string string_value = preference_data[param_name];
          uint8_t uint_value = preference_data[param_name];

          auto result = writeConfigParam(param_name, string_value, uint_value);
          logger.incIndent();
          if (result) {
            logger.printfln(LOG_TYPE::DATA, "Writing '%s' was successful", param_name.c_str());
          } else {
            logger.printfln(LOG_TYPE::ERR, "Writing '%s' failed", param_name.c_str());
            writing_data_successful = false;
          }
          logger.decIndent();
        } else {
          logger.printfln(LOG_TYPE::DATA, "Skipped '%s'", param_name.c_str());
        }
      }
    } else {
      logger.println(LOG_TYPE::DATA, "No 'data' in config");
    }

    logger.decIndent();

    logger.println("Writing gadgets");
    logger.incIndent();

    // Write Gadgets
    if (config.containsKey("gadgets")) {
      JsonArray gadgets_list = config["gadgets"];
      for (auto gadget_data: gadgets_list) {
        if (gadget_data.containsKey("type") && gadget_data.containsKey("name")) {
          auto write_status = writeGadget(gadget_data);

          std::string gadget_name = gadget_data["name"];
          logger.incIndent();
          if (write_status == WriteGadgetStatus::WritingOK) {
            logger.printfln(LOG_TYPE::INFO, "Writing '%s' was successful", gadget_name.c_str());
          } else {
            auto err_msg = writeGadgetStatusToString(write_status);
            logger.printfln(LOG_TYPE::ERR, "Writing '%s' failed: %s", gadget_name.c_str(), err_msg.c_str());
            writing_data_successful = false;
          }
          logger.decIndent();
        } else {
          logger.println(LOG_TYPE::ERR, "'type' or 'name' missing in gadget config");
        }
      }
    } else {
      logger.println(LOG_TYPE::DATA, "No 'gadgets' in config");
    }

    logger.decIndent();
    logger.decIndent();

    return writing_data_successful;
  }

  /**
   * Writes a gadget from the config json body
   * @param json_body JSON-data to save the gadget from
   * @return (Whether writing was successful | Status-Message)
   */
  static gadget_tuple getGadgetDataFromJson(DynamicJsonDocument json_body) {
    auto type = json_body["type"].as<uint8_t>();

    auto name = json_body["name"].as<std::string>();

    uint8_t port0 = 0;
    uint8_t port1 = 0;
    uint8_t port2 = 0;
    uint8_t port3 = 0;
    uint8_t port4 = 0;

    if (json_body.containsKey("ports")) {
      JsonObject ports = json_body["ports"].as<JsonObject>();
      if (ports.containsKey("port0")) {
        port0 = ports["port0"].as<uint8_t>();
      }
      if (ports.containsKey("port1")) {
        port1 = ports["port1"].as<uint8_t>();
      }
      if (ports.containsKey("port2")) {
        port2 = ports["port2"].as<uint8_t>();
      }
      if (ports.containsKey("port3")) {
        port3 = ports["port3"].as<uint8_t>();
      }
      if (ports.containsKey("port4")) {
        port4 = ports["port4"].as<uint8_t>();
      }
    }

    pin_set pins = {port0, port1, port2, port3, port4};

    std::string gadget_config;
    std::string code_config;

    if (json_body.containsKey("config")) {
      gadget_config = json_body["config"].as<std::string>();
    }
    if (json_body.containsKey("codes")) {
      code_config = json_body["codes"].as<std::string>();
    }

    // Create bitfield
    bitfield_set remote_bf = {false, false, false, false, false, false, false, false};

    if (json_body.containsKey("remotes")) {
      JsonObject remote_json = json_body["remotes"].as<JsonObject>();
      if (remote_json.containsKey("gadget")) {
        remote_bf[0] = remote_json["gadget"].as<bool>();
      }
      if (remote_json.containsKey("code")) {
        remote_bf[1] = remote_json["code"].as<bool>();
      }
      if (remote_json.containsKey("event")) {
        remote_bf[2] = remote_json["event"].as<bool>();
      }
    }

    return gadget_tuple(type, remote_bf, pins, name, gadget_config, code_config);
  }

public:

  /**
   * Loads the system config (either from EEPROM or static config)
   * @return The loaded Config
   */
  static std::shared_ptr <Config> loadConfig() {
    #ifdef STATIC_CONFIG_ACTIVE

    return nullptr;

    #else

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

    #endif
  }

  /**
   * Saves the passed config to the EEPROM. Returns false if the Chip runs in static config mode.
   * @param config The config to save
   * @return Whether saving was successful
   */
  static bool saveConfig(Config config) {
    #ifdef STATIC_CONFIG_ACTIVE
    logger.println("Config is not saved: Using static config.");
    return false;
    #else
    logger.println(LOG_TYPE::ERR, "Saving config isn't implemented");
    return false;
    #endif
  }

  /**
   * Creates a Config object from a json config
   * @param config Config json to get data from
   * @return The config if creating was successful, nullptr otherwise
   */
  static std::shared_ptr <Config> createConfigFromJson(DynamicJsonDocument config) {

    if (!checkJsonForKeys(config, {"data", "gadgets"})) {
      return nullptr;
    }

    JsonObject json_config_data = config["data"];
    JsonArray json_gadget_data = config["gadgets"];

    if (!checkJsonForKeys(json_config_data, {"id",
                                             "network_mode",
                                             "irrecv_pin",
                                             "irsend_pin",
                                             "radiorecv_pin",
                                             "radiosend_pin",
                                             "wifi_ssid",
                                             "wifi_pw",
                                             "mqtt_ip,"
                                             "mqtt_port",
                                             "mqtt_user",
                                             "mqtt_pw"})) {
      return nullptr;
    }

    std::vector <gadget_tuple> gadgets;
    for (const auto gadget_data: json_gadget_data) {
      if (checkJsonForKeys(gadget_data, {"name", "type"})) {
        gadget_tuple out_data = getGadgetDataFromJson(gadget_data);
        gadgets.push_back(out_data);
      }
    }

    std::string id = json_config_data["id"];

    NetworkMode network_mode = (NetworkMode) json_config_data["network_mode"].as<uint8_t>();

    uint8_t ir_recv = json_config_data["irrecv_pin"];
    uint8_t ir_send = json_config_data["irsend_pin"];

    uint8_t radio_recv = json_config_data["radiorecv_pin"];
    uint8_t radio_send = json_config_data["radiosend_pin"];

    std::shared_ptr <std::string> wifi_ssid = nullptr;
    std::shared_ptr <std::string> wifi_pw = nullptr;

    std::shared_ptr <IPAddress> mqtt_ip = nullptr;
    std::shared_ptr <uint16_t> mqtt_port = nullptr;

    std::shared_ptr <std::string> mqtt_username = nullptr;
    std::shared_ptr <std::string> mqtt_pw = nullptr;

    if (json_config_data["wifi_ssid"]) {
      wifi_ssid = std::make_shared<std::string>(json_config_data["wifi_ssid"].as<std::string>());
    }

    if (json_config_data["wifi_pw"]) {
      wifi_pw = std::make_shared<std::string>(json_config_data["wifi_pw"].as<std::string>());
    }

    if (json_config_data["mqtt_ip"]) {
      IPAddress buf_ip;
      buf_ip.fromString(json_config_data["mqtt_ip"].as<std::string>().c_str());
      mqtt_ip = std::make_shared<IPAddress>(buf_ip);
    }

    if (json_config_data["mqtt_port"]) {
      mqtt_port = std::make_shared<uint16_t>(json_config_data["mqtt_port"].as<uint16_t>());
    }

    if (json_config_data["mqtt_user"]) {
      mqtt_username = std::make_shared<std::string>(json_config_data["mqtt_user"].as<std::string>());
    }

    if (json_config_data["mqtt_pw"]) {
      mqtt_pw = std::make_shared<std::string>(json_config_data["mqtt_pw"].as<std::string>());
    }

    auto buf_config = Config(id,
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

    return std::make_shared<Config>(buf_config);
  }
};
