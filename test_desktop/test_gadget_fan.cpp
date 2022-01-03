#include "catch.hpp"

#include "../src/test_dummys/gadget_fan_dummy.h"

TEST_CASE("Test Fan", "[Gadget]") {
  GadgetFanDummy fan("yolokopter", 6);

  SECTION("Test Attributes") {
    CHECK(fan.getType() == GadgetType::Fan);
    CHECK(fan.getName() == "yolokopter");
    CHECK(fan.getCharacteristic(CharacteristicIdentifier::status) != nullptr);
    CHECK(fan.getCharacteristic(CharacteristicIdentifier::fan_speed) != nullptr);
  }

  SECTION("Test Characteristics") {
    //TODO: implement
  }
}
