#pragma once

#include "../gadgets/characteristic.h"

class CharacteristicUpdateMeta {
public:
  CharacteristicIdentifier type;
  uint16_t step_val;

  CharacteristicUpdateMeta(CharacteristicIdentifier type, uint16_t step_val);
};
