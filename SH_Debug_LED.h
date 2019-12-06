#ifndef __SH_Debug_LED__
#define __SH_Debug_LED__

#include "SH_Gadget.h"
#include <Arduino.h>

class SH_Debug_LED : public SH_Lamp
{
  private:

    public:
      SH_Debug_LED(const char * name):
        SH_Lamp(name)
        {
        };

      void setStatus(bool new_status)
      {
        Serial.printf("(%s) New Status: %i\n", name, new_status);
        SH_Lamp::setStatus(new_status);
      };

      void setHue(byte new_hue[3])
      {
        Serial.printf("(%s) New Hue: (%d, %d, %d)\n", name, new_hue[0], new_hue[1], new_hue[2]);
        SH_Lamp::setHue(new_hue);
      };

      void setBrightness(byte new_brightness)
      {
        Serial.printf("(%s) New Status: %i\n", name, new_brightness);
        SH_Lamp::setBrightness(new_brightness);
      }
};

#endif