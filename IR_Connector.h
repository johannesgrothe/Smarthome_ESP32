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

    // DynamicJsonDocument send_commands;

    bool decode_ir()
    {
      return false;
    };

    bool send_ir()
    {
      return false;
    };

  public:

    IR_Connector(IR_Gadget * ir_gadget):
      irgadget(ir_gadget)
      {

      };
};

#endif