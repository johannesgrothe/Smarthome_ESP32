#include "sh_lamp_westinghouse_ir.h"

#include <utility>

SH_Lamp_Westinghouse_IR::SH_Lamp_Westinghouse_IR(std::string name) :
  SH_Lamp(std::move(name), SHLampType::ON_OFF) {
}

void SH_Lamp_Westinghouse_IR::refresh() {
  if (gadgetHasChanged()) {
    if (getStatus())
      sendRawIR(lamp_on, 143);
    else
      sendRawIR(lamp_off, 119);
  }
}