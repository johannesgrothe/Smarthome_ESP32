#pragma once

#include "gadget.h"

class Switch: public Gadget {
protected:

  void switchTriggered();

public:
  explicit Switch(std::string name);

};
