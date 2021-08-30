#pragma once

#include "../gadgets/sh_wallswitch.h"

class GadgetWallswitchDummy : public SH_Wallswitch {
public:
  GadgetWallswitchDummy(std::string name);

  void refresh();
};

GadgetWallswitchDummy::GadgetWallswitchDummy(std::string name):
  SH_Wallswitch(name) {}

void GadgetWallswitchDummy::refresh() {}