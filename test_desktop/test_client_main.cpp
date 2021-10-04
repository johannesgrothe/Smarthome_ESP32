#include "catch.hpp"

#include "../src/client_main.h"

TEST_CASE("Test Client Main", "[Main]") {

  SECTION("Test Boot") {
    auto main = ClientMain();
  }
}
