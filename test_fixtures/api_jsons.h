#pragma once

#include <ArduinoJson.h>
#include "../src/gadgets/gadget_enums.h"
#include "../src/console_logger.h"
#include "../src/network_mode.h"

inline DynamicJsonDocument generateCharacteristicStatus() {
  DynamicJsonDocument json(400);
  json["type"] = int(CharacteristicIdentifier::status);
  json["min"] = 0;
  json["max"] = 1;
  json["steps"] = 1;
  json["step_value"] = 1;
  return json;
}

inline DynamicJsonDocument generateCharacteristicFanspeed() {
  DynamicJsonDocument json(400);
  json["type"] = int(CharacteristicIdentifier::fanSpeed);
  json["min"] = 0;
  json["max"] = 100;
  json["steps"] = 4;
  json["step_value"] = 2;
  return json;
}


inline DynamicJsonDocument generateGadgetFan() {
  DynamicJsonDocument json(400);
  json["type"] = int(GadgetIdentifier::fan_westinghouse_ir);
  json["name"] = "test_fan";
  json.createNestedArray("characteristics");
  JsonArray characteristics = json["characteristics"];
  characteristics.add(generateCharacteristicFanspeed());
  characteristics.add(generateCharacteristicStatus());
  return json;
}

inline DynamicJsonDocument generateSystemConfig() {
  DynamicJsonDocument json(400);

  json["id"] = "test_client";
  json["wifi_ssid"] = "yolowifi";
  json["wifi_pw"] = "yolopw";
  json["mqtt_ip"] = "192.168.166.44";
  json["mqtt_port"] = 1884;
  json["mqtt_user"] = "user";
  json["mqtt_pw"] = "pw";
  json["irrecv_pin"] = 1;
  json["irsend_pin"] = 2;
  json["radio_recv_pin"] = 3;
  json["radio_send_pin"] = 4;
  json["network_mode"] = (int) NetworkMode::MQTT;

  return json;
}

inline DynamicJsonDocument generateEventConfig() {
  DynamicJsonDocument json(400);
  auto array = json.createNestedArray("events");
  auto event = array.createNestedObject();
  event["type"] = "test_event";
  auto codes = event.createNestedArray("codes");
  codes.add(123456);
  codes.add(345678);
  logger_i("yolo", "codes size %d", codes.size());
  logger_i("yolo", "events size %d", array.size());
  return json;
}

inline DynamicJsonDocument generateGadgetConfig() {
  DynamicJsonDocument json(400);
  auto array = json.createNestedArray("gadgets");
  auto gadget = array.createNestedObject();

  return json;
}