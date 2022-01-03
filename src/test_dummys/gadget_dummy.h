#pragma once

#include <utility>

#include "../gadgets/gadget.h"

class DummyGadget : public Gadget {
protected:

  void executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) override;

public:

  DummyGadget(std::string name, GadgetType type, std::vector<Characteristic> characteristics);

  void refresh() override;

  void test_updateInitStatus(bool status_update);

  bool test_sendRawIR(const uint16_t raw_data[], uint8_t content_length);

  bool test_sendIR(unsigned long command, uint8_t com_type);

};

DummyGadget::DummyGadget(std::string name, GadgetType type, std::vector<Characteristic> characteristics) :
    Gadget(std::move(name),
           type,
           std::move(characteristics)) {}

void DummyGadget::executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) {}

void DummyGadget::refresh() {}

void DummyGadget::test_updateInitStatus(bool status_update) {
  updateInitStatus(status_update);
}

bool DummyGadget::test_sendRawIR(const uint16_t raw_data[], const uint8_t content_length) {
  return sendRawIR(raw_data, content_length);
}

bool DummyGadget::test_sendIR(const unsigned long command, const uint8_t com_type) {
  return sendIR(command, com_type);
}
