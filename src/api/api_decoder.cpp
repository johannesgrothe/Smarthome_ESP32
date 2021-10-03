#include "api_decoder.h"

GadgetMeta ApiDecoder::decodeGadget(DynamicJsonDocument gadget_data) {
  std::string name = gadget_data["name"];
  GadgetIdentifier gadget_type = GadgetIdentifier(gadget_data["type"].as<int>());
  auto characteristics_array = gadget_data["characteristics"].as<JsonArray>();
  std::vector<CharacteristicMeta> characteristics;
  for (auto characteristic_data: characteristics_array) {
    characteristics.push_back(decodeCharacteristic(characteristic_data));
  }
  return {gadget_type, name, characteristics};
}

CharacteristicMeta ApiDecoder::decodeCharacteristic(DynamicJsonDocument characteristic_data) {
  CharacteristicIdentifier characteristic_type = CharacteristicIdentifier(characteristic_data["type"].as<int>());
  int min_val = characteristic_data["min"];
  int max_val = characteristic_data["max"];
  uint16_t steps = characteristic_data["steps"];
  int step_value = characteristic_data["step_value"];

  return {characteristic_type, min_val, max_val, step_value, steps};
}
