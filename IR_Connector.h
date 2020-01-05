#ifndef __IR_Connector__
#define __IR_Connector__

// #include <Arduino.h>
#include "IR_Gadget.h"
#include "ArduinoJson.h"

//Connector for IR Usage

class IR_Connector
{
  protected:

    IR_Gadget * irgadget;

    // DynamicJsonDocument recv_commands;

    bool decode_ir()
    {
      return false;
    };

    bool send_ir()
    {
      return false;
    };

  public:

    void setIRGadget(IR_Gadget * new_gadget)
    {
      irgadget = new_gadget;
    }

};

#endif