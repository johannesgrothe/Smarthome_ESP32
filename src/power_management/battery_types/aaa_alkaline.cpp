#include "aaa_alkaline.h"

AAA_Alkaline::AAA_Alkaline(uint8_t pwr_pin, uint8_t num_batteries) :
    BatteryManager(pwr_pin),
    num_batteries_(num_batteries) {
  max_voltage_difference_ = (MAX_VOLTAGE - MIN_VOLTAGE) * num_batteries;
}

uint8_t AAA_Alkaline::getBatteryLevel() {
  return uint8_t(calculateBatteryLevel());
}

double AAA_Alkaline::calculateBatteryLevel() const {
  double voltage = getBatteryVoltage();
  if (voltage > (MAX_VOLTAGE * num_batteries_)) {
    return 100;
  } else if (voltage <= (MIN_VOLTAGE * num_batteries_)) {
    return 0;
  }
  double voltage_difference = voltage - (MIN_VOLTAGE * num_batteries_);
  auto charge = (voltage_difference / max_voltage_difference_) * 100;
  return charge;
}
