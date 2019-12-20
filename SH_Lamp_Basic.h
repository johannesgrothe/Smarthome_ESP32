#ifndef __SH_Lamp_Basic__
#define __SH_Lamp_Basic__

#include "SH_Gadget.h"
#include <Arduino.h>

class SH_Lamp_Basic : public SH_Lamp
{
  private:
    uint8_t pin;
    bool default_state;
    


  public:
    SH_Lamp_Basic(const char * name, uint8_t lamp_pin, bool default_lamp_state):
      SH_Lamp(name, ON_OFF),
      pin(lamp_pin),
      default_state(default_lamp_state)
      {
      };
      
    void refresh()
    {
      if (has_changed)
      {
        Serial.printf("%s has changed.", name);
        digitalWrite(pin, (getStatus() != default_state));
      }
      has_changed = false;
    };

    void init()
    {
      pinMode(pin, OUTPUT);
      digitalWrite(pin, default_state);
      return true;
    }
};

#endif