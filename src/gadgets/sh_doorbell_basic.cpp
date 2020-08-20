#include "sh_doorbell_basic.h"

SH_Doorbell_Basic::SH_Doorbell_Basic(const JsonObject& gadget) :
  SH_Doorbell(gadget) {
  if (gadget["pin"] != nullptr) {
    switch_pin_ = gadget["pin"].as<uint8_t>();
    logger.print("Pin: ");
    logger.addln(switch_pin_);
    pinMode(switch_pin_, INPUT);
  } else {
    switch_pin_ = 0;
    logger.println(LOG_TYPE::ERR, "No Pin selected.");
  }
  if (gadget["default_state"] != nullptr) {
    default_state_ = gadget["default_state"].as<uint8_t>();
    logger.print("Default: ");
    logger.addln(default_state_);
    if (switch_pin_ != 0)
      digitalWrite(switch_pin_, default_state_);
  } else {
    default_state_ = false;
    logger.println(LOG_TYPE::ERR, "No Default selected.");
  }
};

void SH_Doorbell_Basic::refresh() {
  if (has_changed) {
    logger.println(getName(), "has changed.");
  }
  has_changed = false;
  if (digitalRead(switch_pin_) == HIGH) {
    triggerEvent();
  }
}