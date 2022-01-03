#include "lamp_westinghouse_ir.h"

#include <utility>

Lamp_WestinghouseIR::Lamp_WestinghouseIR(std::string name) :
    Lamp_OnOff(std::move(name)),
    SimpleHardwareGadget(true) {
}

void Lamp_WestinghouseIR::refresh() {
  if (executeHWChange()) {
    if (getCharacteristicValue(gadget_definitions::CharacteristicIdentifier::status))
      sendRawIR(lamp_on, 143);
    else
      sendRawIR(lamp_off, 119);
  }
}

void
Lamp_WestinghouseIR::executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) {
  setHWChangeStatus(true);
}
