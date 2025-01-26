#pragma once

#include "storage/system_config.h"
#include "connectors/request_gadget.h"

class NetworkLoader {
public:
  static std::shared_ptr<RequestGadget> loadMqtt(const SystemConfig& config);

  static std::shared_ptr<RequestGadget> loadSerial();
};