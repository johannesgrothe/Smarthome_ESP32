#define CATCH_CONFIG_MAIN // don't need to repeat this in more than one cpp file
#include "catch.hpp"


TEST_CASE( "Dummy test", "[Dummy]" ) {

  int yolo_number = 1;

  SECTION( "test if yolo number is 2" ) {
    CHECK(yolo_number == 2);
  }

  SECTION( "test if yolo number is 1" ) {
    CHECK(yolo_number == 1);
  }

  SECTION( "test if yolo number is 4" ) {
    CHECK(yolo_number == 4);
  }
}
