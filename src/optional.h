#pragma once

/**
 * Class to implement the 'Optional' container, since it is not available in the ESP32 compiler toolchain.
 * @tparam T Type of the optional datatype
 */
template <class T>
class Optional {
  T data_;
  bool has_value_;

public:
  Optional():
  data_(),
  has_value_(false) {}

  Optional(T value):
  data_(value),
  has_value_(true) {
    if (value == nullptr) {
      has_value_ = false;
    }
  }

  bool has_value() const {
    return has_value_;
  }
};