#ifndef __SH_Lamp_Basic__
#define __SH_Lamp_Basic__

#include "SH_Gadget.h"
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
      Serial.printf("    => Pin: %d\n", pin);
    } else {
      pin = 0;
      Serial.println("    => [ERR] No Pin selected.");
    }
    if (gadget["default_state"] != nullptr) {
      default_state = gadget["default_state"].as<uint8_t>();
      Serial.printf("    => Default: %d\n", default_state);
    } else {
      default_state = false;
      Serial.println("    => [ERR] No Default selected");
    }
  };

  void refresh() override {
    if (has_changed) {
      Serial.printf("[%s] has changed.\n", name);
      digitalWrite(pin, (getStatus() != default_state));
    }
    has_changed = false;
  };

  bool init() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, default_state);
    return true;
  }
};

#endif