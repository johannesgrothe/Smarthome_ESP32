#include "wallswitch.h"

#include <utility>

void Switch::switchTriggered() {
  // TODO: throw event
}

Switch::Switch(std::string name) :
    Gadget(std::move(name), GadgetType::Wallswitch, std::vector<Characteristic>()) {}

void Switch::executeCharacteristicUpdate(CharacteristicIdentifier, uint16_t) {}
