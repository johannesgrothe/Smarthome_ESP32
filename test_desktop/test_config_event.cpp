#include "catch.hpp"

#include "../src/storage/event_config.h"

TEST_CASE("Test Event Config", "[]") {

  SECTION("Test constructor") {
    EventConfig cfg({{"rem_1_off", {123456789, 987654321}}});
    CHECK(cfg.crc16() != 0);
  }
}
