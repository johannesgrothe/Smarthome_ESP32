#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "code_gadget.h"
#include "request_gadget.h"

class Serial_Gadget : public Request_Gadget {
protected:

  void executeRequestSending(Request * req) override;

  void receiveSerialRequest();

public:

  explicit Serial_Gadget();

  void refresh() override;
};
