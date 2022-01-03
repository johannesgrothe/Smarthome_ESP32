#pragma once

#include "gadget.h"

class Switch: public Gadget {
protected:

  void switchTriggered();

  void executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier, uint16_t) override;

public:
  explicit Switch(std::string name);

};
