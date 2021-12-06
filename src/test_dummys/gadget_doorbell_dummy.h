#pragma once

#include <utility>

#include "../gadgets/doorbell.h"

class GadgetDoorbellDummy : public Doorbell {
public:
  explicit GadgetDoorbellDummy(std::string name);

  void refresh() final;

  void executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) final;
};

GadgetDoorbellDummy::GadgetDoorbellDummy(std::string name): Doorbell(std::move(name)) {}

void GadgetDoorbellDummy::refresh() {}

void GadgetDoorbellDummy::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {}
