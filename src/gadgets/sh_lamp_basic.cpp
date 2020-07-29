#include "sh_lamp_basic.h"

SH_Lamp_Basic::SH_Lamp_Basic(const JsonObject& gadget) :
  SH_Lamp(gadget) {
  if (gadget["pin"] != nullptr) {
    pin = gadget["pin"].as<uint8_t>();
    logger.print("Pin: ");
    pinMode(pin, OUTPUT);
    logger.addln(pin);
  } else {
    pin = 0;
    logger.println(LOG_TYPE::ERR, "No Pin selected.");
  }
  if (gadget["default_state"] != nullptr) {
    default_state = gadget["default_state"].as<uint8_t>();
    logger.print("Default: ");
    logger.addln(default_state);
    if (pin != 0)
      digitalWrite(pin, default_state);
  } else {
    default_state = false;
    logger.println(LOG_TYPE::ERR, "No Default selected.");
  }
  initialized = true;
}

void SH_Lamp_Basic::refresh() {
  if (has_changed) {
    logger.println(getName(), "has changed.");
    digitalWrite(pin, getStatus());
  }
  has_changed = false;
}