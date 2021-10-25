#include "wallswitch_basic.h"

#include "../hardware_controller.h"

void Switch_Basic::refresh() {
  if (HardwareController::digitalReadPin(pin_) != default_state_) {
    unsigned long now = HardwareController::getMillis();
    if (last_action_ + 50 < now || last_action_ > now) {
      switchTriggered();
    }
    last_action_ = now;
  }
}

Switch_Basic::Switch_Basic(std::string name, uint8_t pin, bool default_state) :
    Switch(std::move(name)),
    pin_(pin),
    last_action_(0),
    default_state_(default_state) {
  HardwareController::setPinMode(pin_, INPUT);
}
