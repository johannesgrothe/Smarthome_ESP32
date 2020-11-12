#pragma once

#include "sh_gadget.h"

class SH_Wallswitch: public SH_Gadget {
protected:

  void switchTriggered();

  void executeCharacteristicUpdate(GadgetCharacteristic, int) override;

public:
  explicit SH_Wallswitch(std::string name);

  vector<GadgetCharacteristicSettings> getCharacteristics() override;

  void handleMethodUpdate(GadgetMethod) override;
};
