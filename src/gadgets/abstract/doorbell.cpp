#include "doorbell.h"

Doorbell::Doorbell(const std::string &name) : Gadget(name, GadgetType::Doorbell) {
};

void Doorbell::doorbellTriggered() {
    //  updateCharacteristic("ProgrammableSwitchEvent", true);
    // TODO: event remote needed
}
