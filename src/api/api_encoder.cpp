#include "api_encoder.h"

DynamicJsonDocument ApiEncoder::encodeClient(const ClientMeta &client_data, uint16_t runtime_id) {
  DynamicJsonDocument doc(800);
  doc["runtime_id"] = runtime_id;
  doc["boot_mode"] = int(client_data.boot_mode);
  doc["sw_uploaded"] = client_data.sw_uploaded;
  doc["sw_commit"] = client_data.sw_commit;
  doc["sw_branch"] = client_data.sw_branch;

  std::stringstream api_version_strm;
  api_version_strm << (int) client_data.api_version_major << "." << (int) client_data.api_version_minor << "." << (int) client_data.api_version_bugfix;
  doc["api_version"] = api_version_strm.str();

  doc.createNestedObject("port_mapping");
  JsonObject ports = doc["port_mapping"];
  for (auto &port_data: client_data.port_mapping) {
    std::stringstream port;
    port << std::get<0>(port_data);
    auto pin = std::get<1>(port_data);
    ports[port.str()] = pin;
  }
  return doc;
}

DynamicJsonDocument ApiEncoder::encodeGadget(const GadgetMeta &gadget_data) {
  DynamicJsonDocument doc(1000);
  doc["type"] = int(gadget_data.type);
  doc["id"] = gadget_data.name;
  doc.createNestedArray("characteristics");
  JsonArray characteristics = doc["characteristics"];
  for (auto &characteristic_data: gadget_data.characteristics) {
    characteristics.add(encodeCharacteristic(characteristic_data));
  }
  return doc;
}

DynamicJsonDocument ApiEncoder::encodeCharacteristic(CharacteristicMeta characteristic_data) {
  DynamicJsonDocument doc(1000);
  doc["type"] = int(characteristic_data.type);
  doc["min"] = characteristic_data.min_val;
  doc["max"] = characteristic_data.max_val;
  doc["step_value"] = characteristic_data.step_val;
  doc["steps"] = characteristic_data.steps;
  return doc;
}

DynamicJsonDocument ApiEncoder::encodeSync(const ClientMeta &client_data,
                                           const std::vector<GadgetMeta> &gadget_data,
                                           uint16_t runtime_id) {
  DynamicJsonDocument doc(1500);
  doc["client"] = encodeClient(client_data, runtime_id);
  doc.createNestedArray("gadgets");
  JsonArray gadgets = doc["gadgets"];
  for (const auto &gadget: gadget_data) {
    gadgets.add(encodeGadget(gadget));
  }
  return doc;
}

DynamicJsonDocument ApiEncoder::encodeGadgetUpdate(const GadgetUpdateMeta &data) {
  DynamicJsonDocument doc(1500);
  doc["id"] = data.id;
  doc.createNestedArray("characteristics");
  for (auto c: data.characteristics) {
    auto c_data = doc["characteristics"].createNestedObject();
    c_data["type"] = int(c.type);
    c_data["step_value"] = c.step_val;
  }
  return doc;
}

DynamicJsonDocument ApiEncoder::encodeHeartbeat(uint16_t runtime_id) {
  DynamicJsonDocument doc(100);
  doc["runtime_id"] = runtime_id;
  return doc;
}
