#include "sh_doorbell_basic.h"

#include <utility>

/**
 * Constructor for the SH_Doorbell_Basic
 * @param name Name of the gadget
 * @param pin Pin the doorbell switch is connected to
 * @param default_state The default state of the pin
 */
SH_Doorbell_Basic::SH_Doorbell_Basic(std::string name, uint8_t pin, bool default_state) :
  SH_Doorbell(std::move(name)),
  switch_pin_(pin),
  default_state_(default_state),
  last_event_(0) {
  pinMode(switch_pin_, INPUT);
};

void SH_Doorbell_Basic::refresh() {
  if (digitalRead(switch_pin_) != default_state_) {
    unsigned long now = millis();
    if (now > last_event_ + HW_BOUNCE_DELAY) {
      doorbellTriggered();
    }
    last_event_ = now;
  }
}