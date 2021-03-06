#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "code_gadget.h"
#include "request_gadget.h"

class SerialGadget : public RequestGadget {
protected:

  void executeRequestSending(Request * req) override;

  void receiveSerialRequest();

public:

  explicit SerialGadget();

  void refresh_network() override;
};
