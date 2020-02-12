#ifndef HELPING_STRUCTURES_H
#define HELPING_STRUCTURES_H

#include <ArduinoJson.h>
#include <cstring>

class Mapping_Reference{
protected:
  byte code_count;
  unsigned long codes[15];
  char command_name[25];
public:
  Mapping_Reference() {
  };

  explicit Mapping_Reference(JsonArray data, const char * name) {
    strcpy(command_name, name);
    code_count = data.size() < 15 ? data.size() : 15;
//    Serial.printf("      => Scanning %d codes to Command '%s'\n", code_count, name);
    for (byte k = 0; k < code_count; k++) {
      unsigned long new_code = data[k].as<unsigned long>();
//      Serial.printf("         => 0x%ld\n", new_code);
      codes[k] = new_code;
    }
    printMapping();
  };

  byte getCodeCount() {
    return code_count;
  }

  const char * getName() {
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
    Serial.printf("       => %s: [", &command_name[0]);
    for (byte k = 0; k < code_count; k++) {
      if (k < code_count - 1) {
        Serial.printf("0x%ld, ", codes[k]);
      } else {
        Serial.printf("0x%ld", codes[k]);
      }
    }
    Serial.println("]");
  }

};

#endif //HELPING_STRUCTURES_H
