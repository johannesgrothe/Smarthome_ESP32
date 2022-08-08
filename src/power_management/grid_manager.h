#pragma once

#include <cstdint>
#include "power_manager.h"


class GridManager : public PowerManager {

public:
  explicit GridManager();

  bool refresh() override;
};
