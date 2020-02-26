#ifndef __SH_Lamp_NeoPixel_Basic__
#define __SH_Lamp_NeoPixel_Basic__

#include "sh_lamp.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__

#include <avr/power.h>

#endif

class SH_Lamp_NeoPixel_Basic : public SH_Lamp {
private:
  uint8_t pin;
  uint16_t len;
  Adafruit_NeoPixel led_stripe;

public:

  explicit SH_Lamp_NeoPixel_Basic(JsonObject gadget) :
    SH_Lamp(gadget, 3) {
    if (gadget["pin"] != nullptr) {
      pin = gadget["pin"].as<uint8_t>();
      logger.print("Pin: ");
      logger.addln(pin);
    } else {
      pin = 0;
      logger.println(LOG_ERR,"No Pin selected.");
    }
    if (gadget["length"] != nullptr) {
      len = gadget["length"].as<uint16_t>();
      logger.print("Length: ");
      logger.addln(len);
    } else {
      len = 1;
      logger.println(LOG_ERR,"No Length selected.");
    }
  };

  void refresh() override {
    if (has_changed) {
      logger.printname(name, "has changed.");
      print();
      uint8_t rgb[3];
      getColor(&rgb[0]);
      setColor(rgb[SH_CLR_red], rgb[SH_CLR_green], rgb[SH_CLR_blue]);
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
//    Serial.printf("[%s] Setting Color: (%d, %d, %d)\n", name, r, g, b);
    for (uint16_t k = 0; k < len; k++) {
      led_stripe.setPixelColor(k, Adafruit_NeoPixel::Color(r, g, b));
    }
    led_stripe.show();
    return true;
  }
};

#endif