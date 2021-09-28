#pragma once

#include "../gadgets/gadget_enums.h"
#include "../datatypes.h"

class CharacteristicMeta {
public:
  CharacteristicMeta(CharacteristicIdentifier type, uint16_t min_val, uint16_t max_val, uint16_t step_val, uint16_t steps);

  const CharacteristicIdentifier type;
  const uint16_t min_val;
  const uint16_t max_val;
  const uint16_t step_val;
  const uint16_t steps;
};
