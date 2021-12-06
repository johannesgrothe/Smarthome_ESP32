#include "catch.hpp"

#include "../src/test_dummys/ir_dummy.h"
#include "../src/connectors/ir_gadget.h"

TEST_CASE("Test IR Gadget", "[IR]") {
  uint16_t raw_data[] = {1, 2, 3, 4, 5};
  IR_Gadget test_ir(32, 34);
  CHECK(test_ir.sendIR(2342432435, 4));
  CHECK(test_ir.sendRawIR(raw_data, 5));
  test_ir.refresh();
}
