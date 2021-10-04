#pragma once

#include <ArduinoJson.h>
#include <sstream>

#include "../datatypes.h"
#include "config.h"
#include "../json_check.h"
#include "../ip_container.h"

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

/**
 * Creates a Config object from a json config
 * @param config Config json to get data from
 * @return The config if creating was successful, nullptr otherwise
 */
static std::shared_ptr<Config> createConfigFromJson(DynamicJsonDocument config) {

  if (!checkJsonForKeys(config, {"data", "gadgets"})) {
    return nullptr;
  }

  JsonObject json_config_data = config["data"];
  JsonArray json_gadget_data = config["gadgets"];

  if (!checkJsonForKeys(json_config_data, {"id",
                                           "network_mode",
                                           "irrecv_pin",
                                           "irsend_pin",
                                           "radio_recv_pin",
                                           "radio_send_pin",
                                           "wifi_ssid",
                                           "wifi_pw",
                                           "mqtt_ip",
                                           "mqtt_port",
                                           "mqtt_user",
                                           "mqtt_pw"})) {
    return nullptr;
  }

  std::vector<gadget_tuple> gadgets;
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

  uint8_t radio_recv = json_config_data["radio_recv_pin"];
  uint8_t radio_send = json_config_data["radio_send_pin"];

  std::shared_ptr<std::string> wifi_ssid = nullptr;
  std::shared_ptr<std::string> wifi_pw = nullptr;

  std::shared_ptr<IPContainer> mqtt_ip = nullptr;
  std::shared_ptr<uint16_t> mqtt_port = nullptr;

  std::shared_ptr<std::string> mqtt_username = nullptr;
  std::shared_ptr<std::string> mqtt_pw = nullptr;

  if (json_config_data["wifi_ssid"]) {
    wifi_ssid = std::make_shared<std::string>(json_config_data["wifi_ssid"].as<std::string>());
  }

  if (json_config_data["wifi_pw"]) {
    wifi_pw = std::make_shared<std::string>(json_config_data["wifi_pw"].as<std::string>());
  }

  if (json_config_data["mqtt_ip"]) {
    uint8_t buf_ip[4];
    uint8_t counter = 0;
    std::stringstream sstr;
    for (char c: json_config_data["mqtt_ip"].as<std::string>()) {
      if (c == '.') {
        uint8_t buf_val = 0;
        sstr >> buf_val;
        buf_ip[counter] = buf_val;
        counter ++;
        sstr.clear();
      } else {
        sstr << c;
      }
    }
    mqtt_ip = std::make_shared<IPContainer>(buf_ip[0], buf_ip[1], buf_ip[2], buf_ip[3]);
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
