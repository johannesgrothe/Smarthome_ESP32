#pragma once

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
  has_value_(true) {}

  bool has_value() const {
    return has_value_;
  }
};