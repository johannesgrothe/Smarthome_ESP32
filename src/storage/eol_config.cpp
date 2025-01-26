#include "eol_config.h"

#include <crc_calculator.h>
#include <utility>

EolConfig::EolConfig(const HwVariant variant, std::string serial):variant(variant), serial(std::move(serial)) {}


unsigned short EolConfig::crc16() const {
    CRC16Calculator crc;

    // Add HW Variant
    crc.add(static_cast<uint8_t>(variant));

    // Add serial
    for (const char c: serial) {
        crc.add(static_cast<uint8_t>(c));
    }

    return crc.value();
}