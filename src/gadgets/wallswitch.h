#pragma once

#include "gadget.h"

class Switch: public Gadget {
protected:

  void switchTriggered();

  void executeCharacteristicUpdate(CharacteristicIdentifier, uint16_t) override;

public:
  explicit Switch(std::string name);

};
