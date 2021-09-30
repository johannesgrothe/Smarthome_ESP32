#include "api_encoder.h"

#include <sstream>

DynamicJsonDocument ApiEncoder::encodeClient(const ClientMeta &client_data) {
  DynamicJsonDocument doc(500);
  doc["runtime_id"] = client_data.runtime_id;
  doc["boot_mode"] = int(client_data.boot_mode);
  doc["sw_uploaded"] = client_data.sw_uploaded;
  doc["sw_commit"] = client_data.sw_commit;
  doc["sw_branch"] = client_data.sw_branch;
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
  doc["name"] = gadget_data.name;
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
  doc["step_val"] = characteristic_data.step_val;
  doc["steps"] = characteristic_data.steps;
  return doc;
}

DynamicJsonDocument ApiEncoder::encodeSync(const ClientMeta &client_data, const std::vector<GadgetMeta> &gadget_data) {
  DynamicJsonDocument doc(1500);
  doc["client"] = encodeClient(client_data);
  doc.createNestedArray("gadgets");
  JsonArray gadgets = doc["gadgets"];
  for (const auto& gadget: gadget_data) {
    gadgets.add(encodeGadget(gadget));
  }
  return doc;
}

DynamicJsonDocument ApiEncoder::encodeGadgetUpdate(const GadgetMeta &gadget_data) {
  DynamicJsonDocument doc(1500);
  doc["gadget"] = encodeGadget(gadget_data);
  return doc;
}
