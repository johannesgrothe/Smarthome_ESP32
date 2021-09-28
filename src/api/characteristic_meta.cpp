#include "characteristic_meta.h"

CharacteristicMeta::CharacteristicMeta(CharacteristicIdentifier type, uint16_t min_val, uint16_t max_val,
                                       uint16_t step_val, uint16_t steps) : type(type),
                                                                            min_val(min_val),
                                                                            max_val(max_val),
                                                                            step_val(step_val),
                                                                            steps(steps) {}
