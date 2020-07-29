#pragma once

#include "gadgets/sh_gadget.h"
#include "system_settings.h"

class Gadget_Collection {
private:

  SH_Gadget *gadgets[MAIN_MAX_GADGETS]{};

  byte gadget_count{};

public:

  bool addGadget(SH_Gadget *new_gadget);

  SH_Gadget *getGadget(const char *gadget_name);

  SH_Gadget *getGadget(byte index) const;

  SH_Gadget *operator[](int index) const;

  byte getGadgetCount() const;
};
