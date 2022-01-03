#pragma once

#include "../gadgets/fan.h"

class GadgetFanDummy : public Fan {
public:
  GadgetFanDummy(const std::string& name, uint8_t levels_count);

  void refresh() final;

  void executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) final;
};

GadgetFanDummy::GadgetFanDummy(const std::string& name, uint8_t levels_count):
    Fan(name, levels_count) {}

void GadgetFanDummy::refresh() {}

void GadgetFanDummy::executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) {}
