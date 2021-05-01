#include "mapping_reference.h"

static const char *TAG = "MappingReference";

// TODO: Initialize all fields
Mapping_Reference::Mapping_Reference() {};

Mapping_Reference::Mapping_Reference(JsonArray data, const char *name) {
  strcpy(command_name, name);
  code_count = data.size() < MAPPING_MAX_CODES ? data.size() : MAPPING_MAX_CODES;
  for (byte k = 0; k < code_count; k++) {
    unsigned long new_code = data[k].as<unsigned long>();
    codes[k] = new_code;
  }
  printMapping();
}

byte Mapping_Reference::getCodeCount() const {
  return code_count;
}

const char *Mapping_Reference::getName() {
  return &command_name[0];
}

bool Mapping_Reference::containsCode(unsigned long in_code) {
  for (byte k = 0; k < code_count; k++) {
    if (codes[k] == in_code) {
      return true;
    }
  }
  return false;
}

void Mapping_Reference::printMapping() {
  std::stringstream s_str;
  s_str << command_name << ": [";
  for (byte k = 0; k < code_count; k++) {
    s_str << codes[k];
    if (k < code_count - 1) {
      s_str << ", ";
    }
  }
  s_str << "]";
  logger_i(TAG, s_str.str().c_str());
}
