#include "event_config.h"

#include <utility>
#include "../crc_calculator.h"

EventConfig::EventConfig(std::vector<event_map> event_data):
event_mapping(std::move(event_data)) {}

unsigned short EventConfig::crc16() const {
  CRC16Calculator crc;
  for (auto event_tuple: event_mapping) {
    for (auto chr: std::get<0>(event_tuple)) {
      crc.add(chr);
    }
    for (auto code: std::get<1>(event_tuple)) {
      crc.add(code & 0xFF);
      crc.add((code & 0xFF00) >>8);
      crc.add((code & 0xFF0000) >>16);
      crc.add((code & 0xFF000000) >>32);
    }
  }
  return crc.value();
}
