#pragma once

#include "system_settings.h"

using bitfield_set = std::array<bool, 8>;
using pin_set = std::array<uint8_t, GADGET_PIN_BLOCK_LEN>;
// Tuple to store a gadget config: type, bitfield, pins, name, gadget_config, code_config
using gadget_tuple = std::tuple<uint8_t, bitfield_set, pin_set, std::string, std::string, std::string>;
using status_tuple = std::tuple<bool, std::string>;
