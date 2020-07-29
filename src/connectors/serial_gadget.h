#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "code_gadget.h"
#include "request_gadget.h"

class Serial_Gadget : public Code_Gadget, public Request_Gadget {
protected:

  static bool strContainsHEX(const char *message) {
    short buff_len = strlen(message);
    for (short i = 0; i < buff_len; i++) {
      int charInt = (int) message[i];
      if (!((charInt >= 48 && charInt <= 57) || (charInt >= 65 && charInt <= 70) || (charInt >= 97 && charInt <= 102)))
        return false;
    }
    return true;
  }

  void executeRequestSending(Request *req) override;

public:
  Serial_Gadget();

  explicit Serial_Gadget(JsonObject data);

  void refresh() override;
};
