#include "sh_doorbell_basic.h"

SH_Doorbell_Basic::SH_Doorbell_Basic(JsonObject gadget) :
  SH_Doorbell(gadget) {
  if (gadget["pin"] != nullptr) {
    switch_pin = gadget["pin"].as<uint8_t>();
    logger.print("Pin: ");
    logger.addln(switch_pin);
    pinMode(switch_pin, INPUT);
  } else {
    switch_pin = 0;
    logger.println(LOG_ERR, "No Pin selected.");
  }
  if (gadget["default_state"] != nullptr) {
    default_state = gadget["default_state"].as<uint8_t>();
    logger.print("Default: ");
    logger.addln(default_state);
    if (switch_pin != 0)
      digitalWrite(switch_pin, default_state);
  } else {
    default_state = false;
    logger.println(LOG_ERR, "No Default selected.");
  }
};

void SH_Doorbell_Basic::refresh() {
  if (has_changed) {
    logger.println(getName(), "has changed.");
  }
  has_changed = false;
  if (digitalRead(switch_pin) == HIGH) {
    triggerEvent();
  }
}