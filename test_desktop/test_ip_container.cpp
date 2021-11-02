#include "catch.hpp"

#include "../src/ip_container.h"

TEST_CASE("Test IP Container", "[]") {

  SECTION("Test Equals") {
    IPContainer con1(1, 2, 3, 4);
    IPContainer con2(4, 5, 6, 7);
    CHECK(con1 == con1);
    CHECK(con1 != con2);
    CHECK(con1.toString() != con2.toString());
  }
}