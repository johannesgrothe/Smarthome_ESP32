#include "catch.hpp"

#include "../src/test_dummys/gadget_sensor_temperature_dummy.h"

TEST_CASE("Test Temperature Sensor", "[Gadget]") {
  GadgetSensorTemperatureDummy dummy("STD");
  CHECK(dummy.getName() == "STD");
  CHECK(dummy.getType() == GadgetType::Temp_Humm_Sensor);
  //TODO: setHumidity() & setTemperature()
  auto result = dummy.getCharacteristics();
  CHECK(result[0].type == CharacteristicIdentifier::humidity);
  CHECK(result[1].type == CharacteristicIdentifier::temperature);
  CHECK(result[0].getStepValue() == 0);
  CHECK(result[1].getStepValue() == 0);
}
