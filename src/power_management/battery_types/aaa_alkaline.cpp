#include "aaa_alkaline.h"

AAA_Alkaline::AAA_Alkaline() :
    BatteryManager() {
  max_voltage_difference_ = MAX_VOLTAGE - MIN_VOLTAGE;
}

uint8_t AAA_Alkaline::getBatteryLevel() {
  return uint8_t(calculateBatteryLevel());
}

double AAA_Alkaline::calculateBatteryLevel() const {
  double voltage = getBatteryVoltage();
  if (voltage > MAX_VOLTAGE) {
    return 100;
  } else if (voltage <= MIN_VOLTAGE) {
    return 0;
  }
  double voltage_difference = voltage - MIN_VOLTAGE;
  auto charge = (voltage_difference / max_voltage_difference_) * 100;
  return charge;
}
