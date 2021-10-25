#include "catch.hpp"

#include "../src/test_dummys/gadget_fan_dummy.h"

TEST_CASE("Test Fan", "[Gadget]") {
  GadgetFanDummy fan("yolokopter", 6);

//  fan.setStatus(true);
//  CHECK(fan.getStatus() == 1);
//  fan.toggleStatus();
//  CHECK(fan.getStatus() == 0);
//  fan.setRotationSpeed(74);
//  CHECK(fan.getRotationSpeed() == 74);
//  fan.rotationSpeedUp();
//  fan.rotationSpeedDown();
  auto result = fan.getCharacteristics();
  CHECK(result[0].type == CharacteristicIdentifier::status);
  CHECK(result[1].type == CharacteristicIdentifier::fanSpeed);
//  fan.handleMethodUpdate(GadgetMethod::toggleStatus);
//  CHECK(fan.getStatus() == 0);
  CHECK(result[0].step_value == 1);
  CHECK(result[1].step_value == 74);
  CHECK(result[0].min == 0);
  CHECK(result[1].min == 0);
  CHECK(result[0].max == 1);
  CHECK(result[1].max == 100);
  CHECK(result[0].steps == 1);
  CHECK(result[1].steps == 16);
}
