#include "sh_wallswitch.h"

#include <utility>

void SH_Wallswitch::switchTriggered() {
  // TODO: throw event
}

SH_Wallswitch::SH_Wallswitch(std::string name) :
    Gadget(std::move(name), GadgetType::Wallswitch, std::vector<Characteristic>()) {}

void SH_Wallswitch::executeCharacteristicUpdate(CharacteristicIdentifier, uint16_t) {}
