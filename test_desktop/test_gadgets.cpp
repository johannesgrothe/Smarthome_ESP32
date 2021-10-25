#include "catch.hpp"

#include "../src/console_logger.h"
#include "../src/test_dummys/gadget_dummy.h"

TEST_CASE("Test Gadget", "[Gadget]") {
  DummyGadget gadget("homer", GadgetType::Lightbulb);
  CHECK(!gadget.hasInitError());
  CHECK(gadget.getName() == "homer");
  CHECK(gadget.getType() == GadgetType::Lightbulb);
//  gadget.handleCodeUpdate(3234234552);
}
