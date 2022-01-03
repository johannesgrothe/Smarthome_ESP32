#include "lamp_onoff.h"

#include <utility>

Lamp_OnOff::Lamp_OnOff(std::string name) :
    Gadget(std::move(name),
           GadgetType::Lightbulb,
           {
               Characteristic(gadget_definitions::CharacteristicIdentifier::status,
                              0,
                              1,
                              1,
                              0)}) {}

bool Lamp_OnOff::getStatus() {
  return getCharacteristicValue(gadget_definitions::CharacteristicIdentifier::status) != 0;
}

void Lamp_OnOff::setStatus(bool status) {
  setCharacteristicValue(gadget_definitions::CharacteristicIdentifier::status, status);
}
