#pragma once

#include "power_mode.h"
#include <cstdint>
#include <string>

enum class PowerMode {
  Grid_Mode = 0,
  Battery_Mode = 1
};



class PowerManager {
private:


protected:
  uint8_t battery_level_;
  uint8_t battery_drain_;
  PowerMode mode_;

public:

  explicit PowerManager(PowerMode mode);

  /**
   * Retrieves
   * @return Initialization status
   */
  uint8_t getBatteryLevel() const;

  /**
   * Whether the PowerManagement was successfully initialized and is ready to use
   * @return Initialization status
   */
  uint8_t getBatteryDrain() const;

};
