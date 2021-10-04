#pragma once

#ifndef UNIT_TEST
#include <Arduino.h>
#endif

#include "console_logger.h"

class HardwareController {
private:

public:

  /**
   * Reboots the chip and prints out the given message
   * @param reason The reason to print to the terminal
   */
  static void rebootChip(const std::string &reason, uint8_t delay = 5);
};