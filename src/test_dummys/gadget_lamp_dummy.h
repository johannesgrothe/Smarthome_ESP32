#pragma once

#include <utility>

#include "../gadgets/lamp_rgb.h"

class DummyLamp : public Lamp_RGB {
public:
  explicit DummyLamp(std::string name);

  void refresh() final;

  void executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) final;
};

DummyLamp::DummyLamp(std::string name) :
    Lamp_RGB(std::move(name)) {}

void DummyLamp::refresh(){}

void DummyLamp::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {}
