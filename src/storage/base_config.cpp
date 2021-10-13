#include "base_config.h"

bool BaseConfig::operator==(const BaseConfig &rhs) const {
  return crc16() == rhs.crc16();
}

bool BaseConfig::operator!=(const BaseConfig &rhs) const {
  return !(operator==(rhs));
}
