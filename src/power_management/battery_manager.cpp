#include <esp32-hal-gpio.h>
#include "battery_manager.h"

BatteryManager::BatteryManager(uint8_t pwr_pin) :
    PowerManager(PowerMode::Battery_Mode),
    voltage_read_pin_(pwr_pin) {
}

double BatteryManager::getBatteryVoltage() const {
  auto voltage_reading =  analogRead(voltage_read_pin_);
  auto voltage = (voltage_reading * 3.3) / 4095;
  return voltage;
}

bool BatteryManager::refresh() {
  auto buf = last_charge_;
  last_charge_ = getBatteryLevel();
  if (last_charge_ != buf) {
    return true;
  }
  return false;
}
