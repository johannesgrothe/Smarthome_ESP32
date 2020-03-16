#ifndef __SH_Lamp_Basic__
#define __SH_Lamp_Basic__

#include "sh_lamp.h"
#include <Arduino.h>

class SH_Lamp_Basic : public SH_Lamp {
private:

  uint8_t pin;

  bool default_state;

public:

  explicit SH_Lamp_Basic(JsonObject gadget) :
    SH_Lamp(gadget) {
    if (gadget["pin"] != nullptr) {
      pin = gadget["pin"].as<uint8_t>();
      logger.print("Pin: ");
      pinMode(pin, OUTPUT);
      logger.addln(pin);
    } else {
      pin = 0;
      logger.println(LOG_ERR,"No Pin selected.");
    }
    if (gadget["default_state"] != nullptr) {
      default_state = gadget["default_state"].as<uint8_t>();
      logger.print("Default: ");
      logger.addln(default_state);
      if (pin != 0)
        digitalWrite(pin, default_state);
    } else {
      default_state = false;
      logger.println(LOG_ERR,"No Default selected.");
    }
    initialized = true;
  }

  void refresh() override {
    if (has_changed) {
      logger.println(getName(), "has changed.");
      digitalWrite(pin, getStatus());
    }
    has_changed = false;
  }
};

#endif