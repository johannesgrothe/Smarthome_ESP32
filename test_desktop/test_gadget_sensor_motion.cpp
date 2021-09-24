#include "catch.hpp"

#include "../src/test_dummys/gadget_sensor_motion_dummy.h"

TEST_CASE("Test Motion Sensor", "[Gadget]") {
  GadgetSensorMotionDummy dummy("move");
  CHECK(dummy.getName() == "move");
  CHECK(dummy.getType() == GadgetType::Motion_Sensor);
  auto result = dummy.getCharacteristics();
  CHECK(result[0].characteristic == CharacteristicIdentifier::status);
  CHECK(result[1].characteristic == CharacteristicIdentifier::err_type);
}
