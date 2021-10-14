#include "catch.hpp"

#include "../src/gadget_manager.h"


TEST_CASE("Test Gadget Manager", "[Main]") {
  GadgetManager manager;

  SECTION("Test Refresh") {
    manager.refresh();
  }
}
