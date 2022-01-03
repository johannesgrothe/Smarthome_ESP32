#pragma once

#include "../system/gadget_definitions.h"
#include "../datatypes.h"

class CharacteristicMeta {
public:
  CharacteristicMeta(gadget_definitions::CharacteristicIdentifier type, int min_val, int max_val, int step_val, uint16_t steps);

  const gadget_definitions::CharacteristicIdentifier type;
  const int min_val;
  const int max_val;
  const int step_val;
  const uint16_t steps;
};
