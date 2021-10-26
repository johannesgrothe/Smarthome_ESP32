#include "catch.hpp"

#include "../src/test_dummys/gadget_wallswitch_dummy.h"

TEST_CASE("Test Switch", "[Gadget]") {
  GadgetWallswitchDummy dummy("switchy");
  CHECK(dummy.getName() == "switchy");
  CHECK(dummy.getType() == GadgetType::Switch);
  auto result = dummy.getCharacteristics();
}
