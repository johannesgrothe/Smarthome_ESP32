#include "lamp_basic.h"

#include <utility>

Lamp_Basic::Lamp_Basic(std::string name, uint8_t pin) :
    Lamp_OnOff(std::move(name)),
    SimpleHardwareGadget(true),
    pin_(pin) {}

void Lamp_Basic::refresh() {
  if (executeHWChange()) {
    bool status = getStatus();
    logger_i(getName(), "Has changed: %d", status);
    HardwareController::digitalWritePin(pin_, status);
  }
}

void Lamp_Basic::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {
  setHWChangeStatus(true);
}
