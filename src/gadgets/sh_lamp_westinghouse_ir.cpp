#include "sh_lamp_westinghouse_ir.h"

SH_Lamp_Westinghouse_IR::SH_Lamp_Westinghouse_IR(const JsonObject& gadget) :
  SH_Lamp(gadget, SHLampType::ON_OFF) {
}

void SH_Lamp_Westinghouse_IR::refresh() {
  if (has_changed) {
    if (getStatus())
      sendRawIR(lamp_on, 143);
    else
      sendRawIR(lamp_off, 119);
  }
  has_changed = false;
}