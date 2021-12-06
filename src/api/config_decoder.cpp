#include "config_decoder.h"

#include <sstream>

gadget_tuple ConfigDecoder::decodeGadget(DynamicJsonDocument json_body) {
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

  port_set pins = {port0, port1, port2, port3, port4};

  std::string gadget_config;

  if (json_body.containsKey("config")) {
    gadget_config = json_body["config"].as<std::string>();
  }

  // TODO: implement event mapping

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

  return gadget_tuple(type, remote_bf, pins, name, gadget_config, {});
}

std::shared_ptr<SystemConfig> ConfigDecoder::decodeSystemConfig(DynamicJsonDocument config) {
  if (!JsonValidator::checkKeys(config, {"id",
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

  std::string id = config["id"];

  NetworkMode network_mode = (NetworkMode) config["network_mode"].as<uint8_t>();

  uint8_t ir_recv = config["irrecv_pin"];
  uint8_t ir_send = config["irsend_pin"];

  uint8_t radio_recv = config["radio_recv_pin"];
  uint8_t radio_send = config["radio_send_pin"];

  std::shared_ptr<std::string> wifi_ssid = nullptr;
  std::shared_ptr<std::string> wifi_pw = nullptr;

  std::shared_ptr<IPContainer> mqtt_ip = nullptr;
  std::shared_ptr<uint16_t> mqtt_port = nullptr;

  std::shared_ptr<std::string> mqtt_username = nullptr;
  std::shared_ptr<std::string> mqtt_pw = nullptr;

  if (config["wifi_ssid"]) {
    wifi_ssid = std::make_shared<std::string>(config["wifi_ssid"].as<std::string>());
  }

  if (config["wifi_pw"]) {
    wifi_pw = std::make_shared<std::string>(config["wifi_pw"].as<std::string>());
  }

  if (config["mqtt_ip"]) {
    uint8_t buf_ip[4];
    uint8_t counter = 0;
    std::stringstream ip_strm;
    int buf_val = 0;
    for (char c: config["mqtt_ip"].as<std::string>()) {
      if (c == '.') {
        ip_strm >> buf_val;
        buf_ip[counter] = (uint8_t) buf_val;
        counter++;
        ip_strm = std::stringstream();
      } else {
        ip_strm << c;
      }
    }
    ip_strm >> buf_val;
    buf_ip[counter] = (uint8_t) buf_val;
    mqtt_ip = std::make_shared<IPContainer>(buf_ip[0], buf_ip[1], buf_ip[2], buf_ip[3]);
  }

  if (config["mqtt_port"]) {
    mqtt_port = std::make_shared<uint16_t>(config["mqtt_port"].as<uint16_t>());
  }

  if (config["mqtt_user"]) {
    mqtt_username = std::make_shared<std::string>(config["mqtt_user"].as<std::string>());
  }

  if (config["mqtt_pw"]) {
    mqtt_pw = std::make_shared<std::string>(config["mqtt_pw"].as<std::string>());
  }

  return std::make_shared<SystemConfig>(id,
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
}

std::shared_ptr<EventConfig> ConfigDecoder::decodeEventConfig(DynamicJsonDocument config) {
  std::vector<event_map> events;
  if (!JsonValidator::checkKeys(config, {"events"})) {
    return nullptr;
  }
  for (const auto gadget_data: config["events"].as<JsonArray>()) {
    if (JsonValidator::checkKeys(gadget_data, {"type", "codes"})) {
      std::string id = gadget_data["type"];
      std::vector<unsigned long> codes;
      for (unsigned long code: gadget_data["codes"].as<JsonArray>()) {
        codes.push_back(code);
      }
      event_map out_data(id, codes);
      events.push_back(out_data);
    }
  }
  auto cfg = std::make_shared<EventConfig>(events);
  return cfg;
}

std::shared_ptr<GadgetConfig> ConfigDecoder::decodeGadgetConfig(DynamicJsonDocument config) {
  std::vector<gadget_tuple> gadgets;
  if (!JsonValidator::checkKeys(config, {"gadgets"})) {
    return nullptr;
  }
  for (const auto gadget_data: config["gadgets"].as<JsonArray>()) {
    if (JsonValidator::checkKeys(gadget_data, {"name", "type"})) {
      gadget_tuple out_data = decodeGadget(gadget_data);
      gadgets.push_back(out_data);
    }
  }
  auto cfg = std::make_shared<GadgetConfig>(gadgets);
  return cfg;
}
