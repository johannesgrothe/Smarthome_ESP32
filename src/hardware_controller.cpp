#include "hardware_controller.h"

void HardwareController::rebootChip(const std::string &reason, uint8_t delay) {
  #ifndef UNIT_TEST
  if (!reason.empty()) {
    logger_i("System", "Rebooting Chip because of '%s' in:", reason.c_str());
  } else {
    logger_i("System", "Rebooting Chip in:");
  }
  for (byte k = delay; k > 0; k--) {
    logger_i("System", "%d", k);
    unsigned long end_timestamp = millis() + delay;
    while (millis() < end_timestamp) {}
  }
  ESP.restart();
  #else
  throw std::exception();
  #endif
}
