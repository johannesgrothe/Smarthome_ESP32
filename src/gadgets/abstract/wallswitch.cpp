#include "wallswitch.h"

#include <utility>

void Switch::switchTriggered() {
  // TODO: throw event
}

Switch::Switch(std::string name) :
    Gadget(std::move(name), GadgetType::Switch) {}

