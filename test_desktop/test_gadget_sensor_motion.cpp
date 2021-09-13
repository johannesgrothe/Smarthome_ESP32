#include <stdexcept>
#include <cassert>

#include "../src/test_dummys/gadget_sensor_motion_dummy.h"

void test_sensor_motion() {
  GadgetSensorMotionDummy dummy("move");
  assert(dummy.getName() == "move");
  assert(dummy.getType() == GadgetType::Motion_Sensor);
  auto result = dummy.getCharacteristics();
  assert(result[0].characteristic == CharacteristicIdentifier::status);
  assert(result[1].characteristic == CharacteristicIdentifier::err_type);
}

int main () {
  logger_i("UnitMotionSensor", "started testing of gadget gadget_sensor_motion" );
  test_sensor_motion();
  logger_i("UnitMotionSensor", "test successful");
}

