#pragma once

#include <cstdint>
#include "power_manager.h"


class BatteryManager : public PowerManager {

protected:
  uint8_t voltage_read_pin_;
  uint8_t last_charge_ = 255;

  double getBatteryVoltage() const;

public:
  explicit BatteryManager(uint8_t pwr_pin);

  virtual uint8_t getBatteryLevel() = 0;

  bool refresh() override;
};
