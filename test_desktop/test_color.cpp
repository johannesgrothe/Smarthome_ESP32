#include <stdexcept>
#include <cassert>

#include "../src/console_logger.h"
#include "../src/color.h"

void test_color() {
  byte r = 10;
  byte g = 70;
  byte b = 40;
  Color color(r, g, b);
  //region constructor, RGBtoHSV, RGBtoHSL BEGIN
  std::cout << "testing if constructor was created properly, via RGBColor \n";
  assert(color.getRGB()->getRed() == r);
  assert(color.getRGB()->getGreen() == g);
  assert(color.getRGB()->getBlue() == b);
  std::cout << "was created properly \n";
  //region constructor, RGBtoHSV, RGBtoHSL END

  //region RGBtoHSV BEGIN
  std::cout << "testing RGBtoHSV converter \n";
  assert(color.getHSV()->getHue() == 150);
  assert(color.getHSV()->getSaturation() == 85);
  assert(color.getHSV()->getValue() == 27);
  std::cout << "RGBtoHSV converter tested successfully \n";
  //region RGBtoHSV END

  //region RGBtoHSL BEGIN
  std::cout << "testing RGBtoHSL converter \n";
  assert(color.getHSL()->getHue() == 150);
  assert(color.getHSL()->getSaturation() == 75);
  assert(color.getHSL()->getLightness() == 15);
  std::cout << "RGBtoHSL converter tested successfully \n";
  //region RGBtoHSL END

  //region HSVtoRGB
  std::cout << "setting new HSVColor \n";
  color.setHSV(340, 15, 80);
  std::cout << "testing HSVtoRGB converter \n";
  assert(color.getRGB()->getRed() == 204);
  assert(color.getRGB()->getGreen() == 173);
  assert(color.getRGB()->getBlue() == 183);
  std::cout << "HSVtoRGB converter tested successfully \n";
  //region HSVtoRGB END

  //region HSLtoRGB BEGIN
  std::cout << "setting new HSLColor \n";
  color.setHSL(183, 57, 72);
  assert(color.getRGB()->getRed() == 142);
  assert(color.getRGB()->getGreen() == 220);
  assert(color.getRGB()->getBlue() == 224);
  std::cout << "HSLtoRGB converter tested successfully \n";
  //region HSLtoRGB END


  //region helper functions BEGIN
  std::cout << "testing color.getHue()... \n";
  assert(color.getHue() + 1 == color.getHSL()->getHue());
  std::cout << "successful \n";
  std::cout << "testing color.getBrightness()... \n";
  assert(color.getBrightness() == color.getHSV()->getValue());
  std::cout << "successful \n";
  std::cout << "testing color.setHue()... \n";
  color.setHue(30);
  assert(color.getHue() == 30);
  assert(color.getHSV()->getHue() == 30);
  assert(color.getHSL()->getHue() == 30);
  std::cout << "successful \n";
  std::cout << "testing color.setBrightness()... \n";
  color.setBrightness(40);
  assert(color.getBrightness() == 40);
  assert(color.getHSV()->getValue() == 40);
  std::cout << "successful \n";
  //region helper functions END
}


int main () {
  logger_i("UnitColor", "started testing the color class..." );
  test_color();
  logger_i("UnitColor", "done testing color");
}