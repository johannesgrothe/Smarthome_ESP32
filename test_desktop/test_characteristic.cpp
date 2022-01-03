#include "catch.hpp"

#include "../test_fixtures/api_jsons.h"
#include "../src/gadgets/characteristic.h"


TEST_CASE("Test Characteristic", "[Gadget]") {

  Characteristic characteristic(gadget_definitions::CharacteristicIdentifier::temperature,
                                -40,
                                80,
                                120,
                                0);

  SECTION("Test Attributes") {
    CHECK(characteristic.min == -40);
    CHECK(characteristic.max == 80);
    CHECK(characteristic.steps == 120);
    CHECK(characteristic.getStepValue() == 0);
  }

  SECTION("Test StepValue") {
    characteristic.setStepValue(1);
    CHECK(characteristic.getStepValue() == 1);

    characteristic.setStepValue(30);
    CHECK(characteristic.getStepValue() == 30);

    characteristic.setStepValue(120);
    CHECK(characteristic.getStepValue() == 120);

    characteristic.setStepValue(0);
    CHECK(characteristic.getStepValue() == 0);
  }
}
