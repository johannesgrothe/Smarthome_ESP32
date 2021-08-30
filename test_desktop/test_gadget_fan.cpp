#include <stdexcept>
#include <cassert>

#include "../src/test_dummys/gadget_fan_dummy.h"

void test_fan() {
  GadgetFanDummy fan("yolokopter", 6);
  fan.setStatus(true);
  assert(fan.getStatus() == 1);
  fan.toggleStatus();
  assert(fan.getStatus() == 0);
  fan.setRotationSpeed(74);
  assert(fan.getRotationSpeed() == 74);
  fan.rotationSpeedUp();
  fan.rotationSpeedDown();
  auto result = fan.getCharacteristics();
  assert(result[0].characteristic == CharacteristicIdentifier::status);
  assert(result[1].characteristic == CharacteristicIdentifier::fanSpeed);
  fan.handleMethodUpdate(GadgetMethod::toggleStatus);
  assert(fan.getStatus() == 0);
  assert(result[0].value == 1);
  assert(result[1].value == 74);
  assert(result[0].min == 0);
  assert(result[1].min == 0);
  assert(result[0].max == 1);
  assert(result[1].max == 100);
  assert(result[0].step == 1);
  assert(result[1].step == 16);
}

int main () {
  logger_i("UnitFan", "started testing gadget_fan" );
  test_fan();
  logger_i("UnitFan", "test successful");
}