#pragma once

#include "../gadgets/gadget.h"

class DummyGadget : public Gadget {
protected:

  void executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) override;

public:

  DummyGadget(std::string name, GadgetType type);

  void refresh() override;

};

DummyGadget::DummyGadget(std::string name, GadgetType type) : Gadget(name, type, std::vector<Characteristic>()) {

}

void DummyGadget::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {}

void DummyGadget::refresh() {}

