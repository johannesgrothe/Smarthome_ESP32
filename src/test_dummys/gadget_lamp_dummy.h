#pragma once

#include <utility>

#include "../gadgets/lamp_rgb.h"

class DummyLampRGB : public Lamp_RGB {
private:

  void executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) final;

public:

  explicit DummyLampRGB(std::string name);

  void refresh() final;
};

DummyLampRGB::DummyLampRGB(std::string name) :
    Lamp_RGB(std::move(name)) {}

void DummyLampRGB::refresh(){}

void DummyLampRGB::executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) {}
