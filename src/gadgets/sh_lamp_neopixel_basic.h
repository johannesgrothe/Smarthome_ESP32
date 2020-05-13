#ifndef __SH_Lamp_NeoPixel_Basic__
#define __SH_Lamp_NeoPixel_Basic__

#include "sh_lamp.h"
//#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
//#include <NeoPixelBus.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

class SH_Lamp_NeoPixel_Basic : public SH_Lamp {
private:
  uint8_t pin;
  uint16_t len;
  Adafruit_NeoPixel *led_stripe;

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
      pin = gadget["pin"].as<int>();
      logger.print("Pin: ");
      logger.addln(pin);

      led_stripe = new Adafruit_NeoPixel(len, pin, NEO_GRB + NEO_KHZ800);
      led_stripe->begin();
      setLEDColor(0, 0, 0xFF);
      delay(1000);
      setLEDColor(0, 0xFF, 0);
      delay(1000);
      setLEDColor(0xFF, 0, 0);
      delay(1000);
      setLEDColor(0, 0, 0);
    } else {
      pin = 0;
      logger.println(LOG_ERR, "No Pin selected.");
    }
  };

  void refresh() override {
    if (has_changed) {
      logger.println(getName(), "has changed.");
      logger.incIndent();
//      uint8_t rgb[3];
//      getColor(&rgb[0]);
//      setLEDColor(rgb[0], rgb[1], rgb[2]);

      if (getStatus())
        setLEDColor(0, 0, 240);
      else
        setLEDColor(240, 0, 0);

      logger.decIndent();
    }
    has_changed = false;
  };

  void setLEDColor(int r, int g, int b) {
    logger.print(getName(), "Applying Color: (");
    logger.add(r);
    logger.add(", ");
    logger.add(g);
    logger.add(", ");
    logger.add(b);
    logger.addln(")");

    led_stripe->clear();
    for (int k = 0; k < len; k++) {
      led_stripe->setPixelColor(k, Adafruit_NeoPixel::Color(r, g, b));
    }
    led_stripe->show();
  }
};

#endif