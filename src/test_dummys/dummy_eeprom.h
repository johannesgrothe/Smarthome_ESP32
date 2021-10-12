#pragma once

#include <sstream>

#define EEPROM_LEN_MAX 3950

class DummyEeprom {
public:

  uint8_t storage[EEPROM_LEN_MAX]{};

  uint8_t readByte(int address) {
    return storage[address];
  }

  void writeByte(int address, uint8_t value) {
    storage[address] = value;
  }

  char readChar(int address) {
    return char(storage[address]);
  }

  void writeChar(int address, int8_t value) {
    storage[address] = uint8_t(value);
  }

  void commit() {

  }

  bool begin(int size) {
    return true;
  }

  void clear() {
    for (int i = 0; i < EEPROM_LEN_MAX; i++) {
      storage[i] = 0;
    }
  }

  std::string getContent() {
    std::stringstream sstr;
    uint32_t non_zero = 0;
    for (auto b: storage) {
      if (b != 0) {
        non_zero++;
      }
    }
    sstr << "\n  Not Zero: " << non_zero << " \n  [ ";
    for (auto b: storage) {
      sstr << (int) b << ", ";
    }
    sstr << " ]";
    return sstr.str();
  }
};

inline DummyEeprom EEPROM;
