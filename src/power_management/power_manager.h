#pragma once

#include <cstdint>
#include <string>
#include <esp32-hal-gpio.h>


/**
 * Battery types:
 *
 * AAA:
 *  - Primary:
 *   - Alkaline:
 *    - max Voltage: 1.6V (nominal: 1.45V)
 *    - min Voltage: 0.9V (cutoff)
 *    - capacity: 1000mAh
 *    - typical discharge current: 10mA
 *
 *   - Lithium primary:
 *    - max Voltage: 1.8V (nominal: 1.5V)
 *    - min Voltage: 0.8V (cutoff)
 *    - capacity: 1250mAh
 *    - typical discharge current: 10mA
 *
 *  - Secondary:
 *   - Lithium ion:
 *    - max Voltage: 1.5V (nominal: 1.5V)
 *    - min Voltage: 0.8V (cutoff)
 *    - capacity: 500mAh
 *    - typical discharge current: up to 2000mA
 *
 *   - NiHM:
 *    - max Voltage: 1.3V (nominal: 1.2V)
 *    - min Voltage: 0.9V (cutoff)
 *    - capacity: 1000mAh
 *    - typical discharge current: 200mA
 *
 *
 * */




// #define BATTERY_MAX_VOLTAGE 1.6V
// #define BATTERY_MIN_VOLTAGE 0.9V

enum class PowerMode {
  Grid_Mode = 0,
  Battery_Mode = 1
};

class PowerManager {

protected:
  PowerMode mode_;

public:

  explicit PowerManager(PowerMode mode);

  /**
   * Return the active power mode
   * @return active power mode
   */
  PowerMode getPowerMode() const;

  virtual bool refresh() = 0;

};
