#include "sh_wallswitch.h"

#include <utility>

void SH_Wallswitch::switchTriggered() {
  // TODO: throw event
}

SH_Wallswitch::SH_Wallswitch(std::string name) :
  SH_Gadget(std::move(name), GadgetType::Wallswitch) {}

void SH_Wallswitch::executeCharacteristicUpdate(CharacteristicIdentifier, int) {}

vector<GadgetCharacteristicSettings> SH_Wallswitch::getCharacteristics() {
  return {};
}

void SH_Wallswitch::handleMethodUpdate(GadgetMethod) {}
