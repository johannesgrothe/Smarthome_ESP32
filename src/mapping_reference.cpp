#include "mapping_reference.h"

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

byte Mapping_Reference::getCodeCount() {
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
  logger.print(command_name);
  logger.add(": [");
  for (byte k = 0; k < code_count; k++) {
    logger.add(codes[k]);
    if (k < code_count - 1) {
      logger.add(", ");
    }
  }
  logger.addln("]");
}