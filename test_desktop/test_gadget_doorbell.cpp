#include <stdexcept>
#include <cassert>

#include "../src/test_dummys/gadget_doorbell_dummy.h"

void test_doorbell() {
  GadgetDoorbellDummy dummy("banaphone");
  assert(dummy.getName() == "banaphone");
  assert(dummy.getType() == GadgetType::Doorbell);
  auto result = dummy.getCharacteristics();
}

int main () {
  logger_i("UnitBell", "started testing of gadget_doorbell" );
  test_doorbell();
  logger_i("UnitBell", "test successful");
}
