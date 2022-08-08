#pragma once

#include <cstdint>
#include "../battery_manager.h"

#define MAX_VOLTAGE 1.45
#define MIN_VOLTAGE 0.9

class AAA_Alkaline: public BatteryManager {

protected:
  double max_voltage_difference_;

public:
  explicit AAA_Alkaline();

  uint8_t getBatteryLevel() override;

  double calculateBatteryLevel() const;
};
