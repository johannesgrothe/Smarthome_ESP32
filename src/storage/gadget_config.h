#pragma once

#include <vector>
#include <tuple>
#include <memory>
#include <utility>
#include <string>
#include "../datatypes.h"
#include "base_config.h"

class GadgetConfig: public BaseConfig {
private:

public:
  const std::vector<gadget_tuple> gadgets;

  explicit GadgetConfig(std::vector<gadget_tuple> gadget_data);

  unsigned short crc16() const override;
};