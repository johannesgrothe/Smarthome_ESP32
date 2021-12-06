#include "catch.hpp"

#include "../src/ip_container.h"

TEST_CASE("Test IP Container", "[]") {

  SECTION("Test Equals") {
    IPContainer con1(1, 2, 3, 4);
    IPContainer con2(4, 5, 6, 7);
    CHECK(con1 == con1);
    CHECK(con1 != con2);
  }

  SECTION("Test toString()") {
    IPContainer con1(192, 0, 223, 45);
    CHECK(con1.toString() == "192.0.223.45");
  }

  SECTION("Test subscript") {
    IPContainer con1(192, 0, 223, 45);
    CHECK(con1[0] == 192);
    CHECK(con1[1] == 0);
    CHECK(con1[2] == 223);
    CHECK(con1[3] == 45);
  }

  SECTION("Test getData()") {
    IPContainer con1(192, 0, 223, 45);
    CHECK(con1.getData()[0] == 192);
    CHECK(con1.getData()[1] == 0);
    CHECK(con1.getData()[2] == 223);
    CHECK(con1.getData()[3] == 45);
  }
}
