#include "catch.hpp"

#include "../src/event_manager.h"


TEST_CASE("Test Event Manager", "[Main]") {
  SECTION("Test empty") {
    EventManager manager({});
    CHECK(!manager.hasEvent());
    CHECK(manager.getEvent() == nullptr);
  }
}
