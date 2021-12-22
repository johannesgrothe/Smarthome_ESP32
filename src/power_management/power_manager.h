#pragma once

#include <cstdint>
#include <string>

enum class PowerMode {
  Grid_Mode = 0,
  Battery_Mode = 1
};

class PowerManager {
private:


protected:
  PowerMode mode_;

public:

  explicit PowerManager(PowerMode mode);

  /**
   * Returns current battery level in %
   * @return current battery level
   */
  uint8_t getBatteryLevel() const;

  /**
   * Return the active power mode
   * @return active power mode
   */
  PowerMode getPowerMode() const;

  /**
   * Return current input voltage
   * in_voltage: x, out_voltage: y
   * y = x * 10
   * @return current voltage
   */
  uint8_t getVoltage() const;

};
