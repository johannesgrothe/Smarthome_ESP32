#include "catch.hpp"

#include "../src/test_helpers/validator.h"

TEST_CASE("Test JSON Validator", "[Helpers]") {

  Validator validator;
  nlohmann::json test_json = R"({"status":"everything ok","ack": true})"_json;
  nlohmann::json test_json_broken = R"({"status":"everything ok","ack": 133})"_json;

  DynamicJsonDocument test_json_arduino(7000);
  deserializeJson(test_json_arduino, R"({"status":"everything ok","ack": true})");

  std::stringstream buf_str;
  buf_str << test_json;
  logger_i("Test Validator", "Testing JSON: %s", buf_str.str().c_str());

  SECTION("Test verify") {
    CHECK(validator.validate(test_json, "default_message.json"));
    CHECK(!validator.validate(test_json_broken, "default_message.json"));
    CHECK(!validator.validate(test_json, "api_config_save.json"));
  }

  SECTION("Test verify ArduinoJson") {
    CHECK(validator.validate(test_json_arduino, "default_message.json"));
  }
}
