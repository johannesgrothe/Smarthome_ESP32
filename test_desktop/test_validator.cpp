#include "catch.hpp"

#include "../src/test_helpers/validator.h"

TEST_CASE("Test JSON Validator", "[Helpers]") {

  Validator validator;

  SECTION("Test Empty") {
    CHECK(true);
  }
}
