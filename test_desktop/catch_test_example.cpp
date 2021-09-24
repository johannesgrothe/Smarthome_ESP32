#define CATCH_CONFIG_MAIN // don't need to repeat this in more than one cpp file
#include "catch.hpp"


TEST_CASE( "Dummy test", "[Dummy]" ) {

  int test_number = 1;

  SECTION( "test if number is 2" ) {
    CHECK(test_number == 2);
  }

  SECTION( "test if number is 1" ) {
    CHECK(test_number == 1);
  }

  SECTION( "test if number is 4" ) {
    CHECK(test_number == 4);
  }
}
