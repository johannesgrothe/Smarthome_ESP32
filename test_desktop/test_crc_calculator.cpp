#include "catch.hpp"

#include "../src/crc_calculator.h"

// TODO: please fix the algorithm

TEST_CASE("Test CRC Calculator", "[]") {
  CRC16Calculator crc;

  SECTION("Test zeros") {
    crc.add(0);
    crc.add(0);
    crc.add(0);
    CHECK(crc.value() != 0);
  }

  SECTION("Test 1") {
    crc.add(1);
    CHECK(crc.value() == 61905);
  }

  SECTION("Test 2") {
    crc.add(2);
    CHECK(crc.value() == 49586);
  }

  SECTION("Test 'o'") {
    crc.add('o');
    CHECK(crc.value() == 31929);
  }

  SECTION("Test 'oO'") {
    crc.add('o');
    crc.add('O');
    CHECK(crc.value() == 48944);
  }

  SECTION("Test 1200 byte payload") {
    for (int i = 0; i < 1200; i++) {
      crc.add(i % 0xFF);
    }
    CHECK(crc.value() != 0);
  }
}
