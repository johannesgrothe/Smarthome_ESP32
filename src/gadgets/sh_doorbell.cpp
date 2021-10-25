#include "sh_doorbell.h"

#include <utility>

SH_Doorbell::SH_Doorbell(std::string name) :
    Gadget(std::move(name), GadgetType::Doorbell, {}) {};

void SH_Doorbell::doorbellTriggered() {
//  updateCharacteristic("ProgrammableSwitchEvent", true);
  // TODO: event remote needed
}

void SH_Doorbell::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {}