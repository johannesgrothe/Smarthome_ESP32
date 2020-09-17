#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "code_gadget.h"
#include "request_gadget.h"

class Serial_Gadget : public Code_Gadget, public Request_Gadget {
protected:

  static bool strContainsHEX(std::string message) {
    for (char i : message) {
      int charInt = (int) i;
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
