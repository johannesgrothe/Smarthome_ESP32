#include "catch.hpp"

#include "../src/test_dummys/gadget_sensor_temperature_dummy.h"

TEST_CASE("Test Temperature Sensor", "[Gadget]") {
  GadgetSensorTemperatureDummy dummy("STD");
  CHECK(dummy.getName() == "STD");
  CHECK(dummy.getType() == GadgetType::Temp_Humm_Sensor);
  //TODO: setHumidity() & setTemperature()
  auto result = dummy.getCharacteristics();
  CHECK(result[0].characteristic == CharacteristicIdentifier::humidity);
  CHECK(result[1].characteristic == CharacteristicIdentifier::temperature);
  CHECK(result[0].value == 0);
  CHECK(result[1].value == 0);
}
