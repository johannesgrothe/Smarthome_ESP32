#include "api_decoder.h"
#include "config_decoder.h"

GadgetMeta ApiDecoder::decodeGadget(DynamicJsonDocument gadget_data) {
  std::string name = gadget_data["name"];
  gadget_definitions::GadgetIdentifier gadget_type = gadget_definitions::GadgetIdentifier(gadget_data["type"].as<int>());
  auto characteristics_array = gadget_data["characteristics"].as<JsonArray>();
  std::vector<CharacteristicMeta> characteristics;
  for (auto characteristic_data: characteristics_array) {
    characteristics.push_back(decodeCharacteristic(characteristic_data));
  }
  return {gadget_type, name, characteristics};
}

GadgetUpdateMeta ApiDecoder::decodeGadgetUpdateInfo(DynamicJsonDocument gadget_data) {
  // TODO: make safe
  std::string id = gadget_data["id"];
  auto characteristics_array = gadget_data["characteristics"].as<JsonArray>();
  std::vector<CharacteristicUpdateMeta> characteristics;
  for (auto characteristic_data: characteristics_array) {
    CharacteristicUpdateMeta buf_meta(gadget_definitions::CharacteristicIdentifier(characteristic_data["type"].as<int>()),
                                      characteristic_data["step_value"]);
    characteristics.push_back(buf_meta);
  }
  return {id, characteristics};
}

CharacteristicMeta ApiDecoder::decodeCharacteristic(DynamicJsonDocument characteristic_data) {
  gadget_definitions::CharacteristicIdentifier characteristic_type = gadget_definitions::CharacteristicIdentifier(characteristic_data["type"].as<int>());
  int min_val = characteristic_data["min"];
  int max_val = characteristic_data["max"];
  uint16_t steps = characteristic_data["steps"];
  uint16_t step_value = characteristic_data["step_value"];

  return {characteristic_type, min_val, max_val, step_value, steps};
}

std::shared_ptr<SystemConfig> ApiDecoder::decodeSystemConfig(const DynamicJsonDocument& data) {
  return ConfigDecoder::decodeSystemConfig(data);
}

std::shared_ptr<EventConfig> ApiDecoder::decodeEventConfig(const DynamicJsonDocument& data) {
  return ConfigDecoder::decodeEventConfig(data);
}

std::shared_ptr<GadgetConfig> ApiDecoder::decodeGadgetConfig(const DynamicJsonDocument& data) {
  return ConfigDecoder::decodeGadgetConfig(data);
}
