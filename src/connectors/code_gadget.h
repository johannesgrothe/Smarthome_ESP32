#ifndef __CODE_CONNECTOR_H__
#define __CODE_CONNECTOR_H__

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../console_logger.h"

class Code_Gadget {
protected:

  bool code_gadget_is_ready;

  unsigned long last_command_hex;

  bool has_news;

  void setCommand(unsigned long newCommand, bool changeStatus = true) {
    last_command_hex = newCommand;
    if(changeStatus)
      has_news = true;
  }

public:
  Code_Gadget() :
      code_gadget_is_ready(false),
      last_command_hex(0),
      has_news(false) {
  };

  explicit Code_Gadget(JsonObject data) :
      code_gadget_is_ready(false),
      last_command_hex(0),
      has_news(false) {
  };

  virtual void refresh() = 0;

  bool codeGadgetIsReady() {
    return code_gadget_is_ready;
  }

  bool hasNewCommand() {
    bool buffer = has_news;
    has_news = false;
    return buffer;
  }

  unsigned long getCommand() {
    return last_command_hex;
  }
};

#endif //__CODE_CONNECTOR_H__