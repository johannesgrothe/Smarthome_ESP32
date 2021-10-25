#include "lamp_westinghouse_ir.h"

#include <utility>

SH_Lamp_Westinghouse_IR::SH_Lamp_Westinghouse_IR(std::string name) :
    LampOnOff(std::move(name)),
    SimpleHardwareGadget(true) {
}

void SH_Lamp_Westinghouse_IR::refresh() {
  if (executeHWChange()) {
    if (getCharacteristicValue(CharacteristicIdentifier::status))
      sendRawIR(lamp_on, 143);
    else
      sendRawIR(lamp_off, 119);
  }
}

void
SH_Lamp_Westinghouse_IR::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {
  setHWChangeStatus(true);
}
