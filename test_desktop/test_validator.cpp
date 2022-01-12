#include "catch.hpp"

#include "../src/test_helpers/validator.h"

TEST_CASE("Test JSON Validator", "[Helpers]") {

  Validator validator;
  nlohmann::json test_json = R"({"status":"everything ok","ack": true})";

  std::stringstream buf_str;
  buf_str << test_json;
  logger_i("Test Validator", "Testing JSON: %s", buf_str.str().c_str());

  SECTION("Test verify") {
    CHECK(validator.validate(test_json, "default_message.json"));
  }
}
