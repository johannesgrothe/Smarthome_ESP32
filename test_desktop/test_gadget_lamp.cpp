#include <stdexcept>
#include <cassert>

#include "../src/console_logger.h"
#include "../src/test_dummys/gadget_lamp_dummy.h"

void test_gadget_lamp() {
  uint8_t buffer[3];
  uint8_t r = 0;
  uint8_t g = 1;
  uint8_t b = 2;
  DummyLamp lamp("spongo", SHLampType::CLR_ONLY);
  assert(lamp.getType() == GadgetType::Lightbulb);
  assert(lamp.getName() == "spongo");
  lamp.setStatus(false);
  assert(lamp.getStatus() == 0);
  lamp.toggleStatus();
  assert(lamp.getStatus() == 1);
  lamp.setHue(200);
  assert(lamp.getHue() == 200);
  lamp.setBrightness(50);
  assert(lamp.getBrightness() == 50);
  lamp.setColor(100, 150, 60);
  assert(lamp.getColor(r) == 100);
  assert(lamp.getColor(g) == 150);
  assert(lamp.getColor(b) == 60);
  lamp.getColor(buffer);
  assert(buffer[0] == 100);
  assert(buffer[1] == 150);
  assert(buffer[2] == 60);
  auto result = lamp.getCharacteristics();
  assert(result.size() == 3);
  assert(result[0].characteristic == CharacteristicIdentifier::status);
  assert(result[1].characteristic == CharacteristicIdentifier::hue);
  assert(result[2].characteristic == CharacteristicIdentifier::saturation);
  assert(result[0].max == 1);
  assert(result[1].max == 100);
  assert(result[2].max == 100);
  assert(result[0].min == 0);
  assert(result[1].min == 0);
  assert(result[2].min == 0);
  assert(result[0].step == 1);
  assert(result[1].step == 1);
  assert(result[2].step == 1);
  assert(result[0].value == 1);
  assert(result[1].value == 300);
  assert(result[2].value == 100);
  lamp.handleMethodUpdate(GadgetMethod::toggleStatus);
  assert(lamp.getStatus() == 0);
}

int main () {
  logger_i("test_gadget_lamp", "started testing of gadget_lamp..." );
  test_gadget_lamp();
  logger_i("test_gadget_lamp", "test successful");
}
