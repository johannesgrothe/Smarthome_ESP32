#pragma once

#include <ArduinoJson.h>
#include "../src/gadgets/gadget_enums.h"

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
