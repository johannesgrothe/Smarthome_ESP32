
#include "catch.hpp"

#include "../src/api/characteristic_meta.h"


TEST_CASE("Test Characteristic Meta", "[Container]") {
  CharacteristicMeta meta(CharacteristicIdentifier::fan_speed,
                          0,
                          100,
                          2,
                          4);

  SECTION("Test Constructor") {
    CHECK(meta.type == CharacteristicIdentifier::fan_speed);
    CHECK(meta.min_val == 0);
    CHECK(meta.max_val == 100);
    CHECK(meta.steps == 4);
    CHECK(meta.step_val == 2);
  }

}
