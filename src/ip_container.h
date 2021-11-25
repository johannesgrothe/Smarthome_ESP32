#pragma once

#include <cstdint>
#include <string>

class IPContainer {
private:
  uint8_t data_[4]{};

public:
  IPContainer(uint8_t first, uint8_t second, uint8_t third, uint8_t forth);

//  explicit IPContainer(std::string data);

  const uint8_t *getData() const;

  std::string toString() const;

  bool operator==(const IPContainer &rhs) const;

  bool operator!=(const IPContainer &rhs) const;

  uint8_t operator[](int index) const;
};
