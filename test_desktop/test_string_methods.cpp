#include "catch.hpp"

#include "../src/console_logger.h"
#include "../src/string_methods.h"

TEST_CASE("Test String Methods", "[Helpers]") {
  CHECK(validate_utf8("spongo"));
}
