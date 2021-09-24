#include "catch.hpp"

#include "../src/connectors/event.h"

TEST_CASE("Test Event", "[Event]") {
  Event event("spongo", 7948573847, EventType::StatusOn);

  SECTION("Test Constructor") {
    CHECK(event.getSender() == "spongo");
    CHECK(event.getType() == EventType::StatusOn);
    CHECK(event.getTimestamp() == 7948573847);
  }
}
