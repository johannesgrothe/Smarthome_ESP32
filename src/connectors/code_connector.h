#ifndef __CODE_CONNECTOR_H__
#define __CODE_CONNECTOR_H__

#include <Arduino.h>
#include <ArduinoJson.h>

class Code_Gadget {
protected:

  bool is_initialized;

  unsigned long last_command_hex;

  long last_command;

  char last_command_str[30]{};

  bool has_news;

public:
  Code_Gadget() :
    is_initialized(false),
    last_command_hex(0),
    last_command(0),
    has_news(false) {
  };

  explicit Code_Gadget(JsonObject data) :
    is_initialized(true),
    last_command_hex(0),
    last_command(0),
    has_news(false) {
  };

  virtual void refresh() {
  }

  bool isInitialized() {
    return is_initialized;
  }

  bool hasNewCommand() {
    bool buffer = has_news;
    has_news = false;
    return buffer;
  }

  bool hasHexCommand() {
    return (getCommandHEX() != 0);
  }

  bool hasStrCommand() {
    return last_command_str[0] != '\0';
  }

  unsigned long getCommandHEX() {
    return last_command_hex;
  }

  const char * getCommandStr() {
    return &last_command_str[0];
  }

  uint8_t getCommandLength() {
    if (hasHexCommand()) {
      return 0;
    }
    return strlen(&last_command_str[0]);
  }
};

#endif //__CODE_CONNECTOR_H__