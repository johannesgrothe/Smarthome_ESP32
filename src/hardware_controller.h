#pragma once

#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <chrono>
#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1
#endif

#include "console_logger.h"

class HardwareController {
public:

  /**
   * Reboots the chip and prints out the given message
   * @param reason The reason to print to the terminal
   */
  static void rebootChip(const std::string &reason, uint8_t delay = 5);

  /**
   * Returns the current timestamp in milliseconds
   * @return The current timestamp in milliseconds
   */
  static unsigned long getMillis();

  /**
   * Sets the mode for a hardware pin
   * @param pin Pin to set mode for
   * @param mode Mode to set the pin to
   */
  static void setPinMode(uint8_t pin, uint8_t mode);

  /**
   * Reads digital value from a hardware pin (HIGH or LOW)
   * @param pin Pin to read from
   */
  static bool digitalReadPin(uint8_t pin);

  /**
   * Writes a digital value to a hardware pin (HIGH/LOW)
   * @param pin Pin to write value to
   * @param value Value to write
   */
  static void digitalWritePin(uint8_t pin, bool value);
};