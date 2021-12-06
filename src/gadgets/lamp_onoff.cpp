#include "lamp_onoff.h"

#include <utility>

Lamp_OnOff::Lamp_OnOff(std::string name) :
    Gadget(std::move(name),
           GadgetType::Lightbulb,
           {
               Characteristic(CharacteristicIdentifier::status,
                              0,
                              1,
                              1,
                              0)}) {}

bool Lamp_OnOff::getStatus() {
  return getCharacteristicValue(CharacteristicIdentifier::status) != 0;
}

void Lamp_OnOff::setStatus(bool status) {
  setCharacteristicValue(CharacteristicIdentifier::status, status);
}
