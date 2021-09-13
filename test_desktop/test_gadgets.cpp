#include <stdexcept>
#include <cassert>

#include "../src/console_logger.h"
#include "../src/test_dummys/gadget_dummy.h"

void test_gadgets(){
  DummyGadget gadget("homer", GadgetType::Lightbulb);
  assert(!gadget.hasInitError());
  assert(gadget.getName() == "homer");
  assert(gadget.getType() == GadgetType::Lightbulb);
  gadget.handleCodeUpdate(3234234552);
//  if (gadget.hasIR()){
//    logger_i("Gadget", "already has radio");
//  } else {
//    gadget.setIR();
//  }
//  assert(gadget.hasIR());

}

int main () {
  logger_i("Gadget", "testing gadgets...");
  test_gadgets();
  logger_i("Gadget", "finished test.");
}