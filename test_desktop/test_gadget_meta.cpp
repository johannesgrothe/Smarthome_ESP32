
#include "catch.hpp"

#include "../src/api/gadget_meta.h"


TEST_CASE("Test Gadget Meta", "[Container]") {
  CharacteristicMeta charac_status(CharacteristicIdentifier::status,
                                   0,
                                   1,
                                   1,
                                   0);
  CharacteristicMeta charac_fanspeed(CharacteristicIdentifier::fanSpeed,
                                     0,
                                     100,
                                     4,
                                     2);
  GadgetMeta meta(GadgetIdentifier::fan_westinghouse_ir,
                  "test_fan",
                  {charac_status,
                   charac_fanspeed});

  SECTION("Test Constructor") {
    CHECK(meta.type == GadgetIdentifier::fan_westinghouse_ir);
    CHECK(meta.name == "test_fan");
    CHECK(!meta.characteristics.empty());
  }

}
