#include "mapping_reference.h"

static const char *TAG = "MappingReference";

// TODO: Initialize all fields
Mapping_Reference::Mapping_Reference() {};

// TODO: change to std::string
Mapping_Reference::Mapping_Reference(JsonArray data, const char *name) {
  strcpy(command_name, name);
  code_count = data.size() < MAPPING_MAX_CODES ? data.size() : MAPPING_MAX_CODES;
  for (uint8_t k = 0; k < code_count; k++) {
    unsigned long new_code = data[k].as<unsigned long>();
    codes[k] = new_code;
  }
  printMapping();
}

uint8_t Mapping_Reference::getCodeCount() const {
  return code_count;
}

const char *Mapping_Reference::getName() {
  return &command_name[0];
}

bool Mapping_Reference::containsCode(unsigned long in_code) {
  for (uint8_t k = 0; k < code_count; k++) {
    if (codes[k] == in_code) {
      return true;
    }
  }
  return false;
}

// TODO: what the actual f***
void Mapping_Reference::printMapping() {
  std::stringstream s_str;
  s_str << command_name << ": [";
  for (uint8_t k = 0; k < code_count; k++) {
    s_str << codes[k];
    if (k < code_count - 1) {
      s_str << ", ";
    }
  }
  s_str << "]";
  std::string mapping = s_str.str();
//  logger_i(TAG, mapping.c_str());
}
