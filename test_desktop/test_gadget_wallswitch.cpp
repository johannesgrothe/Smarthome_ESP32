#include <stdexcept>
#include <cassert>

#include "../src/test_dummys/gadget_wallswitch_dummy.h"

void test_wallswitch() {
  GadgetWallswitchDummy dummy("switchy");
  assert(dummy.getName() == "switchy");
  assert(dummy.getType() == GadgetType::Wallswitch);
  auto result = dummy.getCharacteristics();
}

int main () {
  logger_i("UnitWallSwitch", "started testing of gadget_wallswitch" );
  test_wallswitch();
  logger_i("UnitWallSwitch", "test successful");
}

