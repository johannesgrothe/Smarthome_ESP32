#pragma once

#include <vector>
#include <tuple>
#include <memory>
#include <utility>
#include <string>
#include "../datatypes.h"
#include "base_config.h"

class EventConfig: public BaseConfig {
public:

  const std::vector<event_map> event_mapping;

  explicit EventConfig(std::vector<event_map> event_data);

  unsigned short crc16() const override;
};