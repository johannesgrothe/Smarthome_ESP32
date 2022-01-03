#include "catch.hpp"

#include "../src/console_logger.h"
#include "../src/test_dummys/gadget_dummy.h"

TEST_CASE("Test Gadget", "[Gadget]") {
  DummyGadget gadget("homer",
                     GadgetType::Lightbulb,
                     {
                         Characteristic(gadget_definitions::CharacteristicIdentifier::status,
                                        0,
                                        1,
                                        1,
                                        1),
                         Characteristic(gadget_definitions::CharacteristicIdentifier::temperature,
                                        -40,
                                        80,
                                        120,
                                        0)
                     });

  SECTION("Test Attributes") {
    CHECK(gadget.getName() == "homer");
    CHECK(gadget.getType() == GadgetType::Lightbulb);
    CHECK(gadget.getCharacteristics().size() == 2);
  }

  SECTION("Test Init Errors") {
    CHECK(gadget.hasInitError() == false);
    gadget.test_updateInitStatus(true);
    CHECK(gadget.hasInitError() == false);
    gadget.test_updateInitStatus(false);
    CHECK(gadget.hasInitError() == true);
    gadget.test_updateInitStatus(true);
    CHECK(gadget.hasInitError() == true);
  }

  SECTION("Test Characteristic Updates") {
    CHECK(gadget.getCharacteristic(gadget_definitions::CharacteristicIdentifier::status)->getStepValue() == 1);
    CHECK(gadget.handleCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier::status, 0) == true);
    CHECK(gadget.getCharacteristic(gadget_definitions::CharacteristicIdentifier::status)->getStepValue() == 0);
    CHECK(gadget.handleCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier::status, 1) == true);
    CHECK(gadget.getCharacteristic(gadget_definitions::CharacteristicIdentifier::status)->getStepValue() == 1);
    CHECK(gadget.handleCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier::status, 1) == false);
    CHECK(gadget.getCharacteristic(gadget_definitions::CharacteristicIdentifier::status)->getStepValue() == 1);

    CHECK(gadget.getCharacteristic(gadget_definitions::CharacteristicIdentifier::temperature)->getStepValue() == 0);
    CHECK(gadget.handleCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier::temperature, 1) == true);
    CHECK(gadget.getCharacteristic(gadget_definitions::CharacteristicIdentifier::temperature)->getStepValue() == 1);
    CHECK(gadget.handleCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier::temperature, 0) == true);
    CHECK(gadget.getCharacteristic(gadget_definitions::CharacteristicIdentifier::temperature)->getStepValue() == 0);
    CHECK(gadget.handleCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier::temperature, 0) == false);
    CHECK(gadget.getCharacteristic(gadget_definitions::CharacteristicIdentifier::temperature)->getStepValue() == 0);
  }

  SECTION("Test 'changed'-flag") {
    CHECK(gadget.hasChanged() == true);
    CHECK(gadget.hasChanged() == false);
    gadget.handleCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier::status, 0);
    CHECK(gadget.hasChanged() == true);
    CHECK(gadget.hasChanged() == false);
    gadget.handleCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier::status, 0);
    CHECK(gadget.hasChanged() == false);
  }

  SECTION("Test Radio") {
    auto radio = std::make_shared<Radio_Gadget>();
    CHECK(gadget.hasRadio() == false);
    gadget.setRadio(radio);
    CHECK(gadget.hasRadio() == true);
  }

  SECTION("Test IR") {
    auto ir = std::make_shared<IR_Gadget>(2, 3);
    uint16_t raw_data[] = {1,2,3,4,5,6};

    CHECK(gadget.hasIR() == false);
    CHECK(gadget.test_sendIR(1234123, SONY) == false);
    CHECK(gadget.test_sendRawIR(&raw_data[0], 6) == false);
    gadget.setIR(ir);
    CHECK(gadget.hasIR() == true);
    CHECK(gadget.test_sendIR(1234123, SONY) == true);
    CHECK(gadget.test_sendRawIR(&raw_data[0], 6) == true);
  }
}
