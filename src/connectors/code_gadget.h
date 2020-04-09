#ifndef __CODE_CONNECTOR_H__
#define __CODE_CONNECTOR_H__

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../console_logger.h"
#include "../system_timer.h"
#include "../remotes/code_remote.h"

class Code_Gadget {
protected:
  bool code_gadget_is_ready;

  CodeCommand **com;

  bool has_news;

  void setCommand(CodeType type, unsigned long new_command) {
    has_news = true;
    auto *newcom = new CodeCommand(type, new_command, system_timer.getTime());
//    Serial.println(newcom->getCode());
    delay(10);
    com = &newcom;
  }

public:
  Code_Gadget() :
    code_gadget_is_ready(false),
    com(nullptr),
    has_news(false) {
  };

  explicit Code_Gadget(JsonObject data) :
    code_gadget_is_ready(false),
    com(nullptr),
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

  CodeCommand *getCommand() {
    return *com;
  }
};

#endif //__CODE_CONNECTOR_H__