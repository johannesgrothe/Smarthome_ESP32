#ifndef __SH_Lamp_NeoPixel_Basic__
#define __SH_Lamp_NeoPixel_Basic__

#include "sh_lamp.h"
#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include <NeoPixelBus.h>

#ifdef __AVR__

#include <avr/power.h>

#endif

class SH_Lamp_NeoPixel_Basic : public SH_Lamp {
private:
  uint8_t pin;
  uint16_t len;
//  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *led_stripe;

public:

  explicit SH_Lamp_NeoPixel_Basic(JsonObject gadget) :
    SH_Lamp(gadget, CLR_BRI) {
    if (gadget["length"] != nullptr) {
      len = gadget["length"].as<uint16_t>();
      logger.print("Length: ");
      logger.addln(len);
    } else {
      len = 1;
      logger.println(LOG_ERR, "No Length selected.");
    }
    if (gadget["pin"] != nullptr) {
      pin = gadget["pin"].as<uint8_t>();
      logger.print("Pin: ");
      logger.addln(pin);

//      pinMode(pin, OUTPUT);
//      led_stripe = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(len, pin);
      setColor(0, 0, 0xFF);
      delay(500);
      setColor(0, 0xFF, 0);
      delay(500);
      setColor(0xFF, 0, 0);
      delay(500);
      setLEDColor(0, 0, 0);
    } else {
      pin = 0;
      logger.println(LOG_ERR, "No Pin selected.");
    }
  };

  void refresh() override {
    if (has_changed) {
//      logger.printname(name, "has changed.\n");
      uint8_t rgb[3];
      getColor(&rgb[0]);
      setLEDColor(rgb[0], rgb[1], rgb[2]);

//      if (getStatus())
//        setLEDColor(0xFF, 0xFF, 0xFF);
//      else
//        setLEDColor(0, 0, 0);
    }
    has_changed = false;
  };

  bool init() override {
    // pinMode(pin, OUTPUT);
//    led_stripe = Adafruit_NeoPixel(len, pin, NEO_GRB + NEO_KHZ800);
//    led_stripe.begin();
//    led_stripe.clear();
//    setColor(0, 0, 0xFF);
    return true;
  }

  void setLEDColor(uint8_t r, uint8_t g, uint8_t b) {
    Serial.printf("[%s] Setting Color: (%d, %d, %d)\n", name, r, g, b);
    RgbColor clr(r, g, b);
    for (uint16_t k = 0; k < len; k++) {
//      led_stripe->SetPixelColor(k, clr);
    }
//    led_stripe->Show();
  }
};

#endif