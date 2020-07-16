#pragma once

#include "code_gadget.h"

class Radio_Gadget : public Code_Gadget {
protected:

public:
  Radio_Gadget();

  explicit Radio_Gadget(JsonObject);

  void refresh() override;
};
