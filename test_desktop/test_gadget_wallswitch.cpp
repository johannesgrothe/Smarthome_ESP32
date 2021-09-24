#include "catch.hpp"

#include "../src/test_dummys/gadget_wallswitch_dummy.h"

TEST_CASE("Test Wallswitch", "[Gadget]") {
  GadgetWallswitchDummy dummy("switchy");
  CHECK(dummy.getName() == "switchy");
  CHECK(dummy.getType() == GadgetType::Wallswitch);
  auto result = dummy.getCharacteristics();
}
