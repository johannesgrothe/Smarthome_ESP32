#include "sh_doorbell.h"

#include <utility>

SH_Doorbell::SH_Doorbell(std::string name) :
  SH_Gadget(std::move(name), GadgetType::Doorbell) {};

void SH_Doorbell::doorbellTriggered() {
//  updateCharacteristic("ProgrammableSwitchEvent", true);
  // TODO: event remote needed
}

void SH_Doorbell::executeCharacteristicUpdate(GadgetCharacteristic characteristic, int value) {}

vector<GadgetCharacteristicSettings> SH_Doorbell::getCharacteristics() { return {}; };

void SH_Doorbell::handleMethodUpdate(GadgetMethod method) {}
