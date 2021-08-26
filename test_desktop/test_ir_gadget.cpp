#include <stdexcept>
#include <cassert>

#include "../src/console_logger.h"
#include "../src/test_dummys/ir_dummy.h"
#include "../src/connectors/ir_gadget.h"

void test_ir_gadget(){
  uint16_t raw_data[] = {1, 2, 3, 4, 5};
  IR_Gadget test_ir(32, 34);
  assert(test_ir.sendIR(2342432435, 4));
  assert(test_ir.sendRawIR(raw_data, 5));
  test_ir.refresh();
}

int main () {
  logger_i("IR_Gadget", "started testing of IR_Gadget");
  test_ir_gadget();
  logger_i("IR_Gadget", "test successful");
}