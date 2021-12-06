#include "catch.hpp"

#include "../src/connectors/event.h"

TEST_CASE("Test Event", "[Event]") {
  Event event("rem_1_off", "spongo", 79485737);

  SECTION("Test Constructor") {
    CHECK(event.name == "rem_1_off");
    CHECK(event.sender == "spongo");
    CHECK(event.timestamp == 79485737);
  }
}
