#include "catch.hpp"

#include "../src/test_dummys/gadget_sensor_motion_dummy.h"

TEST_CASE("Test Motion Sensor", "[Gadget]") {
  GadgetSensorMotionDummy dummy("move");
  auto characteristics = dummy.getCharacteristics();

  SECTION("Test Constructor") {
    CHECK(dummy.getName() == "move");
    CHECK(dummy.getType() == GadgetType::Motion_Sensor);
  }

  SECTION("Test Status") {
    auto status = characteristics[0];
    CHECK(status.type == CharacteristicIdentifier::status);
  }
}
