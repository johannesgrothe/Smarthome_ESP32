#pragma once

#include <cstdint>
#include <vector>

class CRC16Calculator {
private:

  std::vector<uint8_t> data_;

  static unsigned short gen_crc16(const uint8_t *data, uint64_t size);

public:

  CRC16Calculator();

  /**
   * Adds a value to calculate the checksum for
   * @param value
   */
  void add(uint8_t value);

  /**
   * Gets the checksum out of all the values added so far
   * @return The checksum for all the values added so far
   */
  unsigned short value() const;

};
