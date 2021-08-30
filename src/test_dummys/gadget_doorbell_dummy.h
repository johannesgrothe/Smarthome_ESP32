#pragma once

#include "../gadgets/sh_doorbell.h"

class GadgetDoorbellDummy : public SH_Doorbell {
public:
  GadgetDoorbellDummy(std::string name);

  void refresh();
};

GadgetDoorbellDummy::GadgetDoorbellDummy(std::string name): SH_Doorbell(name) {}

void GadgetDoorbellDummy::refresh() {}