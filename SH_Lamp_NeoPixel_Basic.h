#ifndef __SH_Lamp_NeoPixel_Basic__
#define __SH_Lamp_NeoPixel_Basic__

#include "SH_Gadget.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__

#include <avr/power.h>

#endif

class SH_Lamp_NeoPixel_Basic : public SH_Lamp
{
  private:
    uint8_t pin;
    uint16_t len;
    Adafruit_NeoPixel led_stripe;


  public:
    SH_Lamp_NeoPixel_Basic(const char * name, uint8_t lamp_pin, uint16_t stripe_length):
      SH_Lamp(name, CLR_BRI),
      pin(lamp_pin),
      len(stripe_length)
      {
      };
      
    void refresh()
    {
      if (has_changed)
      {
        Serial.printf("%s has changed.", name);
        uint8_t rgb[3];
        getColor(&rgb[0]);
        for (uint16_t k = 0; k < len; k++)
        {
          led_stripe.setPixelColor(k, Adafruit_NeoPixel::Color(rgb[SH_CLR_red], rgb[SH_CLR_green], rgb[SH_CLR_blue]));
        }
        led_stripe.show();
      }
      has_changed = false;
    };

    bool init()
    {
      pinMode(pin, OUTPUT);
      led_stripe = Adafruit_NeoPixel(len, pin);
      led_stripe.begin();
      return true;
    }
};

#endif