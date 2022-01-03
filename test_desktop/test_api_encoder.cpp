#include "catch.hpp"
#include <ArduinoJson.h>

#include "../src/api/api_encoder.h"


TEST_CASE("Test API Encoder", "[API]") {

  ClientMeta client({{0, 33}},
                    BootMode::Full_Operation,
                    "date",
                    "commit",
                    "branch");

  CharacteristicMeta characteristic(CharacteristicIdentifier::fan_speed,
                                    0,
                                    100,
                                    2,
                                    4);

  GadgetMeta gadget(GadgetIdentifier::fan_westinghouse_ir,
                    "test_gadget",
                    {characteristic});

  SECTION("Encode client") {
    auto json_data = ApiEncoder::encodeClient(client, 1776);

    CHECK(json_data["runtime_id"] == 1776);
    CHECK(json_data["boot_mode"] == int(BootMode::Full_Operation));
    CHECK(json_data["sw_uploaded"] == "date");
    CHECK(json_data["sw_commit"] == "commit");
    CHECK(json_data["sw_branch"] == "branch");

    CHECK(json_data["port_mapping"]["0"] == 33);
  }

  SECTION("Encode characteristic") {
    auto json_data = ApiEncoder::encodeCharacteristic(characteristic);

    CHECK(json_data["type"] == int(CharacteristicIdentifier::fan_speed));
    CHECK(json_data["min"] == 0);
    CHECK(json_data["max"] == 100);
    CHECK(json_data["step_value"] == 2);
    CHECK(json_data["steps"] == 4);
  }

  SECTION("Encode gadget") {
    auto json_data = ApiEncoder::encodeGadget(gadget);

    CHECK(json_data["type"] == int(GadgetIdentifier::fan_westinghouse_ir));
    CHECK(json_data["id"] == "test_gadget");
    CHECK(json_data["characteristics"].size() == 1);
  }

  SECTION("Encode client sync") {
    auto json_data = ApiEncoder::encodeSync(client, {gadget}, 1776);

    CHECK(json_data["client"]["runtime_id"] == 1776);
    CHECK(json_data["gadgets"][0]["id"] == "test_gadget");
  }

  SECTION("Encode gadget sync") {
    auto json_data = ApiEncoder::encodeGadgetUpdate(gadget);

    CHECK(json_data["gadget"]["id"] == "test_gadget");
  }

}
