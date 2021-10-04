#pragma once

#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <random>
#endif

inline int random_int(uint16_t max_value) {
  #ifndef UNIT_TEST
return random(max_value);
  #else
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, max_value);
  return distribution(generator);
  #endif
}
