#ifndef __SH_Lamp_NeoPixel__
#define __SH_Lamp_NeoPixel__

#include "SH_Gadget.h"
#include <Arduino.h>

class SH_Lamp_NeoPixel : public SH_Lamp
{
  private:
    uint8_t pin;


  public:
    SH_Lamp_NeoPixel(const char * name, uint8_t lamp_pin, bool default_lamp_state):
      SH_Lamp(name, CLR_BRI),
      pin(lamp_pin)
      {
      };
      
    void refresh()
    {
      if (has_changed)
      {
        Serial.printf("%s has changed.", name);
      }
      has_changed = false;
    };
};

#endif