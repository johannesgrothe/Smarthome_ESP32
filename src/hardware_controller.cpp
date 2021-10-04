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

unsigned long HardwareController::getMillis() {
  #ifndef UNIT_TEST
  return millis();
  #else
//  using namespace std::chrono;
//  unsigned long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//  return now;
  typedef std::chrono::system_clock Clock;
  auto now = Clock::now();
  auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
  auto fraction = now - seconds;
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
  return milliseconds.count();
  #endif
}

void HardwareController::setPinMode(uint8_t pin, uint8_t mode) {
  #ifndef UNIT_TEST
  logger_d("HardwareController", "Setting pin %d to %s", pin, mode_str.c_str());
  pinMode(pin, mode);
  #else
  std::string mode_str = "OUTPUT";
  if (mode == INPUT) {
    mode_str = "INPUT";
  }
  logger_i("HardwareController", "Setting pin %d to %s", pin, mode_str.c_str());
  #endif
}

bool HardwareController::digitalRead(uint8_t pin) {
  #ifndef UNIT_TEST
  bool val = digitalRead(pin);
  logger_d("HardwareController", "Reading from pin %d: %d", pin, val);
  return val;
  #else
  return false;
  #endif
}

void HardwareController::digitalWrite(uint8_t pin, bool value) {
  #ifndef UNIT_TEST
  bool val = digitalWrite(pin, value);
  logger_d("HardwareController", "Writing value to pin %d: %d", pin, value);
  return val;
  #else
  logger_i("HardwareController", "Writing value to pin %d: %d", pin, value);
  #endif
}
