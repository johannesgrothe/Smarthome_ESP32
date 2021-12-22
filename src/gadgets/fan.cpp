#include "fan.h"

#include <utility>

Fan::Fan(const std::string &name, uint16_t levels_count) :
    Gadget(name,
           GadgetType::Fan,
           {
               Characteristic(CharacteristicIdentifier::status,
                              0,
                              1,
                              1),
               Characteristic(CharacteristicIdentifier::fan_speed,
                              0,
                              100,
                              levels_count)}) {}
