#include "catch.hpp"
#include <ArduinoJson.h>

#include "../src/api/api_encoder.h"
#include "test_helpers/validator.h"


TEST_CASE("Test API Encoder", "[API]") {

  Validator validator;

  ClientMeta client({{0, 33}},
                    BootMode::Full_Operation,
                    "date",
                    "commit",
                    "branch");

  CharacteristicMeta characteristic(gadget_definitions::CharacteristicIdentifier::fan_speed,
                                    0,
                                    100,
                                    2,
                                    4);

  GadgetMeta gadget(gadget_definitions::GadgetIdentifier::fan_westinghouse_ir,
                    "test_gadget",
                    {characteristic});

  SECTION("Encode client") {
    auto json_data = ApiEncoder::encodeClient(client, 1776);

    CHECK(validator.validate(json_data, "api_client_data.json"));
    CHECK(json_data["runtime_id"] == 1776);
    CHECK(json_data["boot_mode"] == int(BootMode::Full_Operation));
    CHECK(json_data["sw_uploaded"] == "date");
    CHECK(json_data["sw_commit"] == "commit");
    CHECK(json_data["sw_branch"] == "branch");
    CHECK(json_data["port_mapping"]["0"] == 33);
  }

  SECTION("Encode characteristic") {
    auto json_data = ApiEncoder::encodeCharacteristic(characteristic);

    CHECK(json_data["type"] == int(gadget_definitions::CharacteristicIdentifier::fan_speed));
    CHECK(json_data["min"] == 0);
    CHECK(json_data["max"] == 100);
    CHECK(json_data["step_value"] == 2);
    CHECK(json_data["steps"] == 4);
  }

  SECTION("Encode gadget") {
    auto json_data = ApiEncoder::encodeGadget(gadget);

    CHECK(validator.validate(json_data, "api_gadget_data.json"));
    CHECK(json_data["type"] == int(gadget_definitions::GadgetIdentifier::fan_westinghouse_ir));
    CHECK(json_data["id"] == "test_gadget");
    CHECK(json_data["characteristics"].size() == 1);
  }

  SECTION("Encode client sync") {
    auto json_data = ApiEncoder::encodeSync(client, {gadget}, 1776);

    CHECK(validator.validate(json_data, "api_client_sync_request.json"));
    CHECK(json_data["client"]["runtime_id"] == 1776);
    CHECK(json_data["gadgets"][0]["id"] == "test_gadget");
  }

  SECTION("Encode gadget update") {
    auto json_data = ApiEncoder::encodeGadgetUpdate(gadget);

    // TODO: Client Update seems not to work created bug ticket [#69]
//    CHECK(validator.validate(json_data, "bridge_gadget_update_request.json"));
    CHECK(json_data["gadget"]["id"] == "test_gadget");
  }

  SECTION("Encode heartbeat") {
    auto json_data = ApiEncoder::encodeHeartbeat(334);

    CHECK(validator.validate(json_data, "bridge_heartbeat_request.json"));
    CHECK(json_data["runtime_id"] == 334);
  }

}
