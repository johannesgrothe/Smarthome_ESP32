#pragma once

#include "console_logger.h"

/**
 * Checks if all of the characters inside of the string are between 32 and 126
 * @param input String to check
 * @return Whether all of the characters are between 32 and 126
 */
inline static bool validate_utf8(const std::string& input) {
  for (char c: input) {
    if (c < 32) {
      logger_e("validate_utf8", "'%c' - '%d'", c, (int) c);
      return false;
    }
    if (c > 126) {
      logger_e("validate_utf8", "'%c' - '%d'", c, (int) c);
      return false;
    }
  }
  return true;
}
