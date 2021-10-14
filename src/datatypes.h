#pragma once

#include "system_settings.h"
#include <array>
#include <string>
#include <tuple>

// Bool array of lenght 8 to represent 8 bits in a byte
using bitfield_set = std::array<bool, 8>;

// Ports needed by the gadget
using port_set = std::array<uint8_t, GADGET_PIN_BLOCK_LEN>;

// Tuple to store a gadget config: type, bitfield, pins, name, gadget_config, code_config
using gadget_tuple = std::tuple<uint8_t, bitfield_set, port_set, std::string, std::string, std::string>;

// Map of event names to ir/radio codes
using event_map = std::tuple<std::string, std::vector<unsigned long>>;
