#include "api_decoder.h"
#include "config_decoder.h"



GadgetUpdateMeta ApiDecoder::decodeGadgetUpdateInfo(DynamicJsonDocument gadget_data) {
  // TODO: make safe
  std::string id = gadget_data["id"];
  const auto characteristics_array = gadget_data["characteristics"].as<JsonArray>();
  // std::vector<CharacteristicUpdateMeta> characteristics;
  // for (auto characteristic_data: characteristics_array) {
  //   CharacteristicUpdateMeta buf_meta(gadget_definitions::CharacteristicIdentifier(characteristic_data["type"].as<int>()),
  //                                     characteristic_data["step_value"]);
  //   characteristics.push_back(buf_meta);
  // }
  return {id};
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
