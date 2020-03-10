#ifndef HELPING_STRUCTURES_H
#define HELPING_STRUCTURES_H

#include <ArduinoJson.h>
#include <cstring>
#include "system_settings.h"
#include "console_logger.h"

class Mapping_Reference {
protected:
  byte code_count;
  unsigned long codes[MAPPING_MAX_CODES];
  char command_name[MAPPING_MAX_COMMAND_NAME_LEN];
public:
  Mapping_Reference() {
  };

  explicit Mapping_Reference(JsonArray data, const char *name) {
    strcpy(command_name, name);
    code_count = data.size() < MAPPING_MAX_CODES ? data.size() : MAPPING_MAX_CODES;
    for (byte k = 0; k < code_count; k++) {
      unsigned long new_code = data[k].as<unsigned long>();
      codes[k] = new_code;
    }
    printMapping();
  };

  byte getCodeCount() {
    return code_count;
  }

  const char *getName() {
    return &command_name[0];
  }

  bool containsCode(unsigned long in_code) {
    for (byte k = 0; k < code_count; k++) {
      if (codes[k] == in_code) {
        return true;
      }
    }
    return false;
  }

  void printMapping() {
    logger.print(command_name);
    logger.add(": [");
    for (byte k = 0; k < code_count; k++) {
      logger.add("0x");
      logger.add(codes[k], HEX);
      if (k < code_count - 1) {
        logger.add(", ");
      }
    }
    logger.addln("]");
  }

};

#endif //HELPING_STRUCTURES_H
