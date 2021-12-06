#include "catch.hpp"

#include "../src/test_dummys/gadget_doorbell_dummy.h"

TEST_CASE("Test Doorbell", "[Gadget]") {
  GadgetDoorbellDummy dummy("banaphone");

  SECTION("Test Constructor") {
    CHECK(dummy.getName() == "banaphone");
    CHECK(dummy.getType() == GadgetType::Doorbell);
  }
}
