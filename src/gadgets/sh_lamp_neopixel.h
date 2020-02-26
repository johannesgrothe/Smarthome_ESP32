#ifndef __SH_Lamp_NeoPixel__
#define __SH_Lamp_NeoPixel__

#include "sh_lamp.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class SH_Lamp_NeoPixel : public SH_Lamp {
private:
  uint8_t pin;
  uint16_t len;
  Adafruit_NeoPixel led_stripe;


public:
  explicit SH_Lamp_NeoPixel(JsonObject gadget) :
    SH_Lamp(gadget) {
    if (gadget["pin"] != nullptr) {
      pin = gadget["pin"].as<uint8_t>();
    } else {
      pin = 0;
    }
    if (gadget["length"] != nullptr) {
      len = gadget["length"].as<uint16_t>();
    } else {
      len = 1;
    }
  };

  void refresh() override {
    if (has_changed) {
      Serial.printf("[%s] has changed.\n", name);
      print();
      uint8_t rgb[3];
      getColor(&rgb[0]);
      setColor(rgb[SH_CLR_red], rgb[SH_CLR_green], rgb[SH_CLR_blue]);
      print();
    }
    has_changed = false;
  };

  bool init() override {
    // pinMode(pin, OUTPUT);
    led_stripe = Adafruit_NeoPixel(len, pin, NEO_GRB + NEO_KHZ800);
    led_stripe.begin();
    led_stripe.clear();
    setColor(0, 0, 0xFF);
    return true;
  }

  bool setLEDColor(uint8_t r, uint8_t g, uint8_t b) {
    Serial.printf("[%s] Setting Color: (%d, %d, %d)\n", name, r, g, b);
    // Adafruit_NeoPixel::Color clr(r, g, b);
    for (uint16_t k = 0; k < len; k++) {
      led_stripe.setPixelColor(k, Adafruit_NeoPixel::Color(r, g, b));
    }
    led_stripe.show();
    return true;
  }
};

#endif