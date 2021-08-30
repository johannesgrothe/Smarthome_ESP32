#pragma once

#include "../gadgets/sh_fan.h"

class GadgetFanDummy : public SH_Fan {
public:
  GadgetFanDummy(std::string name, uint8_t levels_count);

  void refresh();
};

GadgetFanDummy::GadgetFanDummy(std::string name, uint8_t levels_count):
  SH_Fan(name, levels_count) {}

void GadgetFanDummy::refresh() {}
