#include "catch.hpp"

#include "../src/gadget_factory.h"

TEST_CASE("Test Gadget Factory", "[Gadget]") {
  GadgetFactory factory(nullptr, nullptr);

  SECTION("Test Gadget Creation") {
    DynamicJsonDocument doc(5);
    auto test_g = factory.createGadget(gadget_definitions::GadgetIdentifier::doorbell_basic,
                                       {3, 0, 0, 0, 0},
                                       "test_doorbell",
                                       doc.as<JsonObject>(),
                                       {{"event1", {{3, 55}, {5, 66}}}});
    CHECK(test_g != nullptr);
  }
}
