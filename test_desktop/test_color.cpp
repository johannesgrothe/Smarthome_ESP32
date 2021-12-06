#include "catch.hpp"

#include "../src/console_logger.h"
#include "../src/color.h"

TEST_CASE("Test Color", "[Color]") {
  byte r = 10;
  byte g = 70;
  byte b = 40;
  Color color(r, g, b);

  SECTION("Test Constructor") {
    CHECK(color.getRGB()->getRed() == r);
    CHECK(color.getRGB()->getGreen() == g);
    CHECK(color.getRGB()->getBlue() == b);
  }

  SECTION("Test RGBtoHSV") {
    CHECK(color.getHSV()->getHue() == 150);
    CHECK(color.getHSV()->getSaturation() == 85);
    CHECK(color.getHSV()->getValue() == 27);
  }

  SECTION("Test RGBtoHSL") {
    CHECK(color.getHSL()->getHue() == 150);
    CHECK(color.getHSL()->getSaturation() == 75);
    CHECK(color.getHSL()->getLightness() == 15);
  }

  SECTION("Test setting new HSVColor") {
    color.setHSV(340, 15, 80);
    CHECK(color.getRGB()->getRed() == 204);
    CHECK(color.getRGB()->getGreen() == 173);
    CHECK(color.getRGB()->getBlue() == 183);
  }

  SECTION("Test setting new HSLColor") {
    color.setHSL(183, 57, 72);
    CHECK(color.getRGB()->getRed() == 142);
    CHECK(color.getRGB()->getGreen() == 220);
    CHECK(color.getRGB()->getBlue() == 224);
  }

  SECTION("Test getHue() and getBrightness()") {
    assert(color.getHue() == color.getHSL()->getHue());
    assert(color.getBrightness() == color.getHSV()->getValue());
  }

  SECTION("Test setHue()") {
    color.setHue(30);
    assert(color.getHue() == 30);
    assert(color.getHSV()->getHue() == 30);
    assert(color.getHSL()->getHue() == 30);
  }

  SECTION("Test setBrightness()") {
    color.setBrightness(40);
    assert(color.getBrightness() == 40);
    assert(color.getHSV()->getValue() == 40);
  }
}
