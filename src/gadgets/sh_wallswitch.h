#pragma once

#include "gadget.h"

class SH_Wallswitch: public Gadget {
protected:

  void switchTriggered();

  void executeCharacteristicUpdate(CharacteristicIdentifier, uint16_t) override;

public:
  explicit SH_Wallswitch(std::string name);

};
