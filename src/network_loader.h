#pragma once

#include "storage/config.h"
#include "connectors/request_gadget.h"

class NetworkLoader {
private:
  static std::shared_ptr<RequestGadget> loadMqtt(Config config);

  static std::shared_ptr<RequestGadget> loadSerial();

public:
  static std::shared_ptr<RequestGadget> loadNetwork(const Config& config, NetworkMode mode);
};