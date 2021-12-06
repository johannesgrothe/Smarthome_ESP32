#include "doorbell.h"

#include <utility>

Doorbell::Doorbell(std::string name) :
    Gadget(std::move(name), GadgetType::Doorbell, {}) {};

void Doorbell::doorbellTriggered() {
//  updateCharacteristic("ProgrammableSwitchEvent", true);
  // TODO: event remote needed
}
