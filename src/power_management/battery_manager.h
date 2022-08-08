#pragma once

#include <cstdint>
#include "power_manager.h"
#include "pin_profile.h"


class BatteryManager : public PowerManager {

protected:
  uint8_t voltage_read_pin_ = PWR_PIN;
  uint8_t last_charge_ = 255;

  double getBatteryVoltage() const;

public:
  explicit BatteryManager();

  virtual uint8_t getBatteryLevel() = 0;

  bool refresh() override;
};
