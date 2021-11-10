#include "catch.hpp"

#include "../src/console_logger.h"
#include "../src/test_dummys/gadget_lamp_dummy.h"

TEST_CASE("Test Lamp", "[Gadget]") {
  DummyLampRGB lamp("spongo");

  SECTION("Test Attributes") {
    CHECK(lamp.getType() == GadgetType::Lightbulb);
    CHECK(lamp.getName() == "spongo");
    CHECK(lamp.getCharacteristic(CharacteristicIdentifier::status) != nullptr);
    CHECK(lamp.getCharacteristic(CharacteristicIdentifier::saturation) != nullptr);
    CHECK(lamp.getCharacteristic(CharacteristicIdentifier::brightness) != nullptr);
    CHECK(lamp.getCharacteristic(CharacteristicIdentifier::hue) != nullptr);
  }

  SECTION("Test Status") {
    lamp.handleCharacteristicUpdate(CharacteristicIdentifier::status, 0);
    logger_i("test", "step_val = %d", lamp.getCharacteristic(CharacteristicIdentifier::status)->getStepValue());
    CHECK(lamp.getCharacteristic(CharacteristicIdentifier::status)->getStepValue() == 0);
    lamp.handleCharacteristicUpdate(CharacteristicIdentifier::status, 1);
    logger_i("test", "step_val = %d", lamp.getCharacteristic(CharacteristicIdentifier::status)->getStepValue());
    CHECK(lamp.getCharacteristic(CharacteristicIdentifier::status)->getStepValue() == 1);
  }

//  lamp.setStatus(false);
//  CHECK(lamp.getStatus() == 0);
//  lamp.toggleStatus();
//  CHECK(lamp.getStatus() == 1);
//  lamp.setHue(200);
//  CHECK(lamp.getHue() == 200);
//  lamp.setBrightness(50);
//  CHECK(lamp.getBrightness() == 50);
//  lamp.setColor(100, 150, 60);
//  CHECK(lamp.getColor(r) == 100);
//  CHECK(lamp.getColor(g) == 150);
//  CHECK(lamp.getColor(b) == 60);
//  lamp.getColor(buffer);
//  CHECK(buffer[0] == 100);
//  CHECK(buffer[1] == 150);
//  CHECK(buffer[2] == 60);
//  auto result = lamp.getCharacteristics();
//  CHECK(result.size() == 3);
//  CHECK(result[0].type == CharacteristicIdentifier::status);
//  CHECK(result[1].type == CharacteristicIdentifier::hue);
//  CHECK(result[2].type == CharacteristicIdentifier::saturation);
//  CHECK(result[0].max == 1);
//  CHECK(result[1].max == 100);
//  CHECK(result[2].max == 100);
//  CHECK(result[0].min == 0);
//  CHECK(result[1].min == 0);
//  CHECK(result[2].min == 0);
//  CHECK(result[0].steps == 1);
//  CHECK(result[1].steps == 1);
//  CHECK(result[2].steps == 1);
//  CHECK(result[0].step_value == 1);
//  CHECK(result[1].step_value == 300);
//  CHECK(result[2].step_value == 100);
}
