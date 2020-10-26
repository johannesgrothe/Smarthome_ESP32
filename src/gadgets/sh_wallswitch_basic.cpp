#include "sh_wallswitch_basic.h"

void SH_Wallswitch_Basic::refresh() {
  if (digitalRead(pin_) != default_state_) {
    unsigned long now = millis();
    if (last_action_ + 50 < now || last_action_ > now) {
      switchTriggered();
    }
    last_action_ = now;
  }
}

SH_Wallswitch_Basic::SH_Wallswitch_Basic(std::string name, uint8_t pin, bool default_state) :
  SH_Wallswitch(std::move(name)),
  pin_(pin),
  last_action_(0),
  default_state_(default_state) {
  pinMode(pin_, INPUT);
}
