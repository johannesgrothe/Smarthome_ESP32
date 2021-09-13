#include "console_logger.h"

/**
 * Checks if all of the characters inside of the string are between 32 and 126
 * @param input String to check
 * @return Whether all of the characters are between 32 and 126
 */
bool validate_utf8(std::string input) {
  for (char c: input) {
    if (c < 32) {
      logger_e("TEST", "'%c' - '%d'", c, (int) c);
      return false;
    }
    if (c > 126) {
      logger_e("TEST", "'%c' - '%d'", c, (int) c);
      return false;
    }
  }
  return true;
}