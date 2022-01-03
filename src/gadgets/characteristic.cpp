#include "characteristic.h"

Characteristic::Characteristic(gadget_definitions::CharacteristicIdentifier type, int min, int max, uint16_t step_count,
                               uint16_t default_step_value):
                               default_step_value_(default_step_value),
                               step_value_(default_step_value_),
                               type(type),
                               min(min),
                               max(max),
                               steps(step_count) {}

uint16_t Characteristic::getStepValue() const {
  return step_value_;
}

void Characteristic::setStepValue(uint16_t value) {
  step_value_ = value;
}

uint16_t Characteristic::getDefaultStepValue() const {
  return default_step_value_;
}

//int Characteristic::getTrueValue() const {
//  return min + ((max-min) / steps * step_value_);
//}

//void Characteristic::setTrueValue(int value) {
//  step_value_ = (max-min)
//}
