#pragma once

#include "../gadgets/characteristic.h"

class CharacteristicUpdateMeta {
public:
  gadget_definitions::CharacteristicIdentifier type;
  uint16_t step_val;

  CharacteristicUpdateMeta(gadget_definitions::CharacteristicIdentifier type, uint16_t step_val);
};
