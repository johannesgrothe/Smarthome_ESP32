#pragma once

#include "gadget_enums.h"

class GadgetCharacteristicSettings {
public:
  const CharacteristicIdentifier type;
  const int min;
  const int max;
  const int steps;
  const long step_value;

  GadgetCharacteristicSettings(CharacteristicIdentifier characteristic, int min, int max, int step, long value):
      type(characteristic),
      min(min),
      max(max),
      steps(step),
      step_value(value) {}
};
