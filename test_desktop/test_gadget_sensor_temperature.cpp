#include <stdexcept>
#include <cassert>

#include "../src/test_dummys/gadget_sensor_temperature_dummy.h"

void test_sensor_temperature() {
  GadgetSensorTemperatureDummy dummy("STD");
  assert(dummy.getName() == "STD");
  assert(dummy.getType() == GadgetType::Temp_Humm_Sensor);
  //TODO: setHumidity() & setTemperature()
  auto result = dummy.getCharacteristics();
  assert(result[0].characteristic == CharacteristicIdentifier::humidity);
  assert(result[1].characteristic == CharacteristicIdentifier::temperature);
  assert(result[0].value == 0);
  assert(result[1].value == 0);
}

int main () {
  logger_i("UnitTemperatureSensor", "started testing of gadget_sensor_temperature" );
  test_sensor_temperature();
  logger_i("UnitTemperatureSensor", "test successful");
}