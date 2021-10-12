#pragma once

#include <vector>
#include <tuple>
#include <memory>
#include <utility>
#include <string>
#include "../datatypes.h"

class GadgetConfig {
private:

public:
  const std::vector<gadget_tuple> gadgets;

  explicit GadgetConfig(std::vector<gadget_tuple> gadget_data);
};