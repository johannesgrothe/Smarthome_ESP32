#pragma once

#include "gadget_enums.h"

class GadgetCharacteristicSettings {
public:
  const CharacteristicIdentifier characteristic;
  const int min;
  const int max;
  const int step;
  const long value;

  GadgetCharacteristicSettings(CharacteristicIdentifier characteristic, int min, int max, int step, long value):
  characteristic(characteristic),
  min(min),
  max(max),
  step(step),
  value(value) {}
};
