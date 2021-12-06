#pragma once

#include "../gadgets/gadget_enums.h"
#include "../datatypes.h"

class CharacteristicMeta {
public:
  CharacteristicMeta(CharacteristicIdentifier type, int min_val, int max_val, int step_val, uint16_t steps);

  const CharacteristicIdentifier type;
  const int min_val;
  const int max_val;
  const int step_val;
  const uint16_t steps;
};
