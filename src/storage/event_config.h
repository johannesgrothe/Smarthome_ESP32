#pragma once

#include <vector>
#include <tuple>
#include <memory>
#include <utility>
#include <string>
#include "../datatypes.h"

class EventConfig {
private:

  const std::vector<event_map> event_mapping;

public:

  explicit EventConfig(std::vector<event_map> event_data);

  unsigned short crc16() const;
};