#pragma once

#include "../json_check.h"

//#define STATIC_CONFIG_ACTIVE

#ifdef STATIC_CONFIG_ACTIVE
#include "static_storage.h"
#else

#include "eeprom_storage.h"

#endif

#include "config.h"
#include "../datatypes.h"
#include "../status_codes.h"

/**
 * System storage class handling EEPROM saving
 */
class System_Storage {
private:

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

  /**
   * Loads the system config (either from EEPROM or static config)
   * @return The loaded Config
   */
  static std::shared_ptr <Config> loadConfig() {
    #ifdef STATIC_CONFIG_ACTIVE

    DynamicJsonDocument static_json_config(5000);

    auto err = deserializeJson(static_json_config, STATIC_CONFIG_STR);
    if (err != DeserializationError::Ok) {
      logger.printfln(LOG_TYPE::ERR, "Could not load static config: Deserialization Error");
      return nullptr;
    }

    auto config = createConfigFromJson(static_json_config);

    return config;

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
};
