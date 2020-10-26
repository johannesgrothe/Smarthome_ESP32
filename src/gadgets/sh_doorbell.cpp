#include "sh_doorbell.h"

#include <utility>

SH_Doorbell::SH_Doorbell(std::string name) :
  SH_Gadget(std::move(name), GadgetType::Doorbell) {};

void SH_Doorbell::doorbellTriggered() {
  updateCharacteristic("ProgrammableSwitchEvent", true);
  // TODO: event remote needed
}

void SH_Doorbell::handleCharacteristicUpdate(const char *characteristic, int value) {}

bool SH_Doorbell::getCharacteristics(char *buffer) { return false; };

void SH_Doorbell::handleMethodUpdate(GadgetMethod method) {}
