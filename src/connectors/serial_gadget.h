#pragma once

#include <ArduinoJson.h>
#include "request_gadget.h"

#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include "../test_dummys/dummy_serial.h"
#endif

class SerialGadget : public RequestGadget {
protected:

  void executeRequestSending(std::shared_ptr<Request> req) override;

  void receiveSerialRequest();

public:

  explicit SerialGadget();

  void refresh_network() override;
};
