#pragma once

#include <ArduinoJson.h>
#include "request_gadget.h"

class SerialGadget : public RequestGadget {
protected:

  void executeRequestSending(std::shared_ptr<Request> req) override;

  void receiveSerialRequest();

public:

  explicit SerialGadget();

  void refresh_network() override;
};
