#include "crc_calculator.h"

// https://stackoverflow.com/questions/10564491/function-to-calculate-a-crc16-checksum

CRC16Calculator::CRC16Calculator() :
    data_() {}

void CRC16Calculator::add(uint8_t value) {
  data_.push_back(value);
}

unsigned short CRC16Calculator::value() const {
  return gen_crc16(data_.data(), data_.size());
}

unsigned short CRC16Calculator::gen_crc16(const uint8_t *data, uint64_t size) {
  unsigned char x;
  unsigned short crc = 0xFFFF;

  while (size--) {
    x = crc >> 8 ^ *data++;
    x ^= x >> 4;
    crc = (crc << 8) ^ ((x << 12)) ^ ((x << 5)) ^ (x);
  }
  crc &= 0xffff;
  return crc;
}
