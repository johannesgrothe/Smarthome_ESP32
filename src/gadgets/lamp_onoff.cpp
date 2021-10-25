#include "lamp_onoff.h"

#include <utility>

LampOnOff::LampOnOff(std::string name) :
    Gadget(std::move(name),
           GadgetType::Lightbulb,
           {
               Characteristic(CharacteristicIdentifier::status,
                              0,
                              1,
                              1,
                              0)}) {}
