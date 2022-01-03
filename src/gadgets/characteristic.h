#pragma once

#include "../system/gadget_definitions.h"

#include <cstdint>

class Characteristic {
  const int default_step_value_;
  uint16_t step_value_;
public:
  const gadget_definitions::CharacteristicIdentifier type;
  const int min;
  const int max;
  const uint16_t steps;

  Characteristic(gadget_definitions::CharacteristicIdentifier type, int min, int max, uint16_t step_count, uint16_t default_step_value = 0);

  uint16_t getStepValue() const;

  void setStepValue(uint16_t value);

  uint16_t getDefaultStepValue() const;

//  int getTrueValue() const;

//  void setTrueValue(int value);
};
