#pragma once

#include <vector>
#include <tuple>
#include <utility>
#include "../datatypes.h"
#include "base_config.h"

class EventConfig final : public BaseConfig {
public:

  const std::vector<event_map> event_mapping;

  explicit EventConfig(std::vector<event_map> event_data);

  unsigned short crc16() const override;
};