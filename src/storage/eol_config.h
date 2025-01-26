#pragma once

#include <string>

#include "base_config.h"
#include "temp_variants.h"

class EolConfig final : public BaseConfig {
public:
    HwVariant variant;

    std::string serial;

    explicit EolConfig(HwVariant variant, std::string serial);

    unsigned short crc16() const override;
};
