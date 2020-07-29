#pragma once

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
  Mapping_Reference();

  explicit Mapping_Reference(JsonArray data, const char *name);

  byte getCodeCount();

  const char *getName();

  bool containsCode(unsigned long in_code);

  void printMapping();
};
