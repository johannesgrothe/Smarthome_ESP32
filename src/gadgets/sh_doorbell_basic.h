#ifndef __SH_Doorbell_Basic__
#define __SH_Doorbell_Basic__

#include "sh_doorbell.h"

class SH_Doorbell_Basic : public SH_Doorbell {
protected:

  byte switch_pin;

  bool default_state;

public:

  explicit SH_Doorbell_Basic(JsonObject gadget) :
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

  void refresh() override {
    if (has_changed) {
      logger.println(getName(), "has changed.");
    }
    has_changed = false;
    if (digitalRead(switch_pin) == HIGH) {
      triggerEvent();
    }
  }
};

#endif //__SH_Doorbell_Basic__