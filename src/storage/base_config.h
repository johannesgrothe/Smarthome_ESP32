#pragma once

class BaseConfig {
public:
  virtual unsigned short crc16() const = 0;

  bool operator==(const BaseConfig &rhs) const;

  bool operator!=(const BaseConfig &rhs) const;
};
