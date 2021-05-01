#pragma once

#include "sh_gadget.h"

class SH_Wallswitch: public SH_Gadget {
protected:

  void switchTriggered();

  void executeCharacteristicUpdate(CharacteristicIdentifier, int) override;

public:
  explicit SH_Wallswitch(std::string name);

  std::vector<GadgetCharacteristicSettings> getCharacteristics() override;

  void handleMethodUpdate(GadgetMethod) override;
};
