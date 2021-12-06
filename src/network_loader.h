#pragma once

#include "storage/system_config.h"
#include "connectors/request_gadget.h"

class NetworkLoader {
private:
  static std::shared_ptr<RequestGadget> loadMqtt(const SystemConfig& config);

  static std::shared_ptr<RequestGadget> loadSerial();

public:
  static std::shared_ptr<RequestGadget> loadNetwork(const SystemConfig &config, NetworkMode mode);
};