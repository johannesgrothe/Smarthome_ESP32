#define CATCH_CONFIG_MAIN // don't need to repeat this in more than one cpp file

#include "catch.hpp"

#include "../test_fixtures/gadget_jsons.h"
#include "../src/api/api_decoder.h"


TEST_CASE("Test API Decoder", "[API]") {

  auto c_fanspeed_json = generateCharacteristicFanspeed();
  auto c_status_json = generateCharacteristicStatus();
  auto g_fan_json = generateGadgetFan();

  SECTION("Decode characteristic") {
    auto characteristic_meta = ApiDecoder::decodeCharacteristic(c_fanspeed_json);
    CHECK(characteristic_meta.type == CharacteristicIdentifier::fanSpeed);
    CHECK(characteristic_meta.min_val == 0);
    CHECK(characteristic_meta.max_val == 100);
    CHECK(characteristic_meta.step_val == 2);
    CHECK(characteristic_meta.steps == 4);
    }

  SECTION("Decode gadget") {
    auto gadget_meta = ApiDecoder::decodeGadget(g_fan_json);
    CHECK(gadget_meta.type == GadgetIdentifier::fan_westinghouse_ir);
    CHECK(gadget_meta.name == "test_fan");
    CHECK(gadget_meta.characteristics.size() == 2);
  }

}
