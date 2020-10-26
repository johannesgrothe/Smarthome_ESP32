#pragma once

#include "sh_gadget.h"

class SH_Wallswitch: public SH_Gadget {
protected:

  void switchTriggered();

public:
  explicit SH_Wallswitch(std::string name);

  void handleCharacteristicUpdate(const char *, int) override;

  bool getCharacteristics(char *) override;

  void handleMethodUpdate(GadgetMethod) override;
};
