#include "eol_config.h"

#include <utility>

EolConfig::EolConfig(const HwVariant variant, std::string serial):variant(variant), serial(std::move(serial)) {}
