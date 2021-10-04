#pragma once

class DummyEeprom {
public:

  uint8_t storage[10000]{};

  uint8_t readByte(int index) {
    return storage[index];
  }

  void writeByte(uint8_t data, int index) {
    storage[index] = data;
  }

  char readChar(int index) {
    return char(storage[index]);
  }

  void writeChar(char data, int index) {
    storage[index] = uint8_t(data);
  }

  void commit() {

  }

  bool begin(int size) {
    return true;
  }

};

inline DummyEeprom EEPROM;
