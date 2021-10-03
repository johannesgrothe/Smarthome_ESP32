#include "characteristic_meta.h"

CharacteristicMeta::CharacteristicMeta(CharacteristicIdentifier type, int min_val, int max_val,
                                       int step_val, uint16_t steps) : type(type),
                                                                       min_val(min_val),
                                                                       max_val(max_val),
                                                                       step_val(step_val),
                                                                       steps(steps) {}
