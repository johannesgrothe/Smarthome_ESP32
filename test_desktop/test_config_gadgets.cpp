#include "catch.hpp"

#include "../src/storage/gadget_config.h"

TEST_CASE("Test Gadget Config", "[]") {

  SECTION("Test constructor") {
    GadgetConfig cfg({});
  }
}
