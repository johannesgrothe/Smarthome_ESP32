#include "sh_wallswitch.h"

#include <utility>

void SH_Wallswitch::switchTriggered() {
  // TODO: throw event
}

SH_Wallswitch::SH_Wallswitch(std::string name) :
  SH_Gadget(std::move(name), GadgetType::Wallswitch) {}

void SH_Wallswitch::handleCharacteristicUpdate(const char *, int) {}

bool SH_Wallswitch::getCharacteristics(char *) {
  return false;
}

void SH_Wallswitch::handleMethodUpdate(GadgetMethod) {}
