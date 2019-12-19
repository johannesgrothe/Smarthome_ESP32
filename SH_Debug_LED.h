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
};

#endif