#include "sh_fan.h"

#include <utility>

SH_Fan::SH_Fan(const std::string &name, uint16_t levels_count) :
    Gadget(name,
           GadgetType::Fan,
           {
                  Characteristic(CharacteristicIdentifier::status,
                                 0,
                                 1,
                                 1),
                  Characteristic(CharacteristicIdentifier::fanSpeed,
                                 0,
                                 100,
                                 levels_count)}),
    SimpleHardwareGadget(true) {}

void SH_Fan::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {
  setHWChangeStatus(true);
}
