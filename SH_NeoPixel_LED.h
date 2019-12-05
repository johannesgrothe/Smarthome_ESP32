#include "SH_Gadget.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

class SH_NeoPixel_LED : SH_Lamp
{
  private:
    Adafruit_NeoPixel led_stripe;
    uint8_t stripe_len;
    uint8_t status;
    uint8_t brightness;
    uint8_t brightness_now;
    uint8_t color[3];
    uint8_t color_now[3];
    char name[5];

    public:
      SH_LED():
        color(),
        color_now(),
        isInitialized(false),
        status(0),
        brightness(100),
        name()
        {
        };
      
};