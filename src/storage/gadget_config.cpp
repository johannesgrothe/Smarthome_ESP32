#include "gadget_config.h"
#include "../crc_calculator.h"

GadgetConfig::GadgetConfig(std::vector<gadget_tuple> gadget_data) :
    gadgets(std::move(gadget_data)) {}

unsigned short GadgetConfig::crc16() const {
  CRC16Calculator crc;
  for (auto gadget: gadgets) {
    auto gadget_ident = std::get<0>(gadget);
//    auto remote_bf = std::get<1>(gadget);
    auto ports = std::get<2>(gadget);
    auto name = std::get<3>(gadget);
    auto gadget_config_str = std::get<4>(gadget);
    auto event_map = std::get<5>(gadget);

    crc.add(gadget_ident);

    for (auto port: ports) {
      crc.add(port);
    }

    for (char c: name) {
      crc.add((uint8_t) c);
    }

    for (char c: gadget_config_str) {
      crc.add((uint8_t) c);
    }

    for (gadget_event_map t: event_map) {
      for (char c: std::get<0>(t)) {
        crc.add((uint8_t) c);
      }

      std::vector<gadget_mapping_tuple> mappings = std::get<1>(t);

      for (gadget_mapping_tuple pair: mappings) {
        uint16_t characteristic = std::get<0>(pair);
        uint16_t value = std::get<0>(pair);

        crc.add(characteristic & 0xFF);
        crc.add(characteristic >> 8);

        crc.add(value & 0xFF);
        crc.add(value >> 8);
      }
    }
  }
  return crc.value();
}
